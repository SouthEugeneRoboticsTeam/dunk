#include <mrpt/hwdrivers/CGenericSensor.h>
#include <mrpt/obs/CObservation2DRangeScan.h>
#include <mrpt/slam/CMetricMapBuilderICP.h>
#include <mrpt/config/CConfigFile.h>
#include <mrpt/io/CFileGZInputStream.h>
#include <mrpt/io/CFileGZOutputStream.h>
#include <mrpt/io/CFileOutputStream.h>
#include <mrpt/system/os.h>
#include <mrpt/system/filesystem.h>
#include <string>
#include <vector>
#include <thread>
#include <signal.h>
#include "json.h"
#include "UDPClient.h"

#define ROBORIO_HOST "10.25.21.2"
#define ROBORIO_PORT 5800

using namespace mrpt;
using namespace mrpt::poses;
using namespace mrpt::io;
using namespace std;

// Forward declaration
void MapBuilding_ICP_Live(const string& INI_FILENAME);

bool allThreadsMustExit = false;

void ExitHandler(int s) {
  allThreadsMustExit = true;
  exit(0);
}

int main(int argc, char** argv) {
  using namespace mrpt::system;

  signal(SIGINT, ExitHandler);

  try {
    bool showHelp = argc > 1 && !os::_strcmp(argv[1], "--help");

    // Process arguments:
    if (argc < 2 || showHelp) {
      printf("Usage: %s <config_file.ini>\n\n", argv[0]);
      if (!showHelp) {
        mrpt::system::pause();
        return -1;
      } else {
        return 0;
      }
    }

    const string INI_FILENAME = string(argv[1]);
    ASSERT_FILE_EXISTS_(INI_FILENAME);

    // Run:
    MapBuilding_ICP_Live(INI_FILENAME);

    // pause();
    return 0;
  } catch(exception& e) {
    setConsoleColor(CONCOL_RED, true);
    cerr << "Program finished due to an exception!" << endl;
    setConsoleColor(CONCOL_NORMAL, true);

    cerr << e.what() << endl;

    mrpt::system::pause();
    return -1;
  } catch(...) {
    setConsoleColor(CONCOL_RED, true);
    cerr << "Program finished due to an untyped exception!" << endl;
    setConsoleColor(CONCOL_NORMAL, true);

    mrpt::system::pause();
    return -1;
  }
}

// Sensor thread
mrpt::hwdrivers::CGenericSensor::TListObservations global_list_obs;
mutex cs_global_list_obs;

struct TThreadParams {
  mrpt::config::CConfigFile* cfgFile;
  string section_name;
};

void SensorThread(TThreadParams params) {
  using namespace mrpt::hwdrivers;
  using namespace mrpt::system;
  try {
    string driver_name = params.cfgFile->read_string(
      params.section_name, "driver", "", true);
    CGenericSensor::Ptr sensor =
      CGenericSensor::createSensorPtr(driver_name);
    if (!sensor)
      throw runtime_error(
        string("***ERROR***: Class name not recognized: ") +
        driver_name);

    // Load common & sensor specific parameters:
    sensor->loadConfig(*params.cfgFile, params.section_name);
    cout << format("[thread_%s] Starting...", params.section_name.c_str())
       << " at " << sensor->getProcessRate() << " Hz" << endl;

    ASSERTMSG_(
      sensor->getProcessRate() > 0,
      "process_rate must be set to a valid value (>0 Hz).");
    const int process_period_ms =
      mrpt::round(1000.0 / sensor->getProcessRate());

    sensor->initialize();
    while (!allThreadsMustExit) {
      const TTimeStamp t0 = now();
      sensor->doProcess();
      // Get new observations
      CGenericSensor::TListObservations lstObjs;
      sensor->getObservations(lstObjs);
      {
        lock_guard<mutex> lock(cs_global_list_obs);
        global_list_obs.insert(lstObjs.begin(), lstObjs.end());
      }
      lstObjs.clear();
      // wait for the process period:
      TTimeStamp t1 = now();
      double At = timeDifference(t0, t1);
      int At_rem_ms = process_period_ms - At * 1000;
      if (At_rem_ms > 0)
        this_thread::sleep_for(chrono::milliseconds(At_rem_ms));
    }
    sensor.reset();
    cout << format("[thread_%s] Closing...", params.section_name.c_str())
       << endl;
  } catch(exception& e) {
    cerr << "[SensorThread]  Closing due to exception:\n"
       << e.what() << endl;
    allThreadsMustExit = true;
  } catch(...) {
    cerr << "[SensorThread] Untyped exception! Closing." << endl;
    allThreadsMustExit = true;
  }
}

void MapBuilding_ICP_Live(const string& INI_FILENAME) {
  auto client = UDPClient(ROBORIO_HOST, ROBORIO_PORT);
  MRPT_START

  using namespace mrpt::slam;
  using namespace mrpt::obs;
  using namespace mrpt::maps;
  
  auto client = UDPClient(ROBORIO_HOST, ROBORIO_PORT);

  mrpt::config::CConfigFile iniFile(INI_FILENAME);

  // Load sensor params from section: "LIDAR_SENSOR"
  thread hSensorThread;
  {
    TThreadParams threParms;
    threParms.cfgFile = &iniFile;
    threParms.section_name = "LIDAR_SENSOR";
    cout << "\n\n==== Launching LIDAR grabbing thread ==== \n";
    hSensorThread = thread(SensorThread, threParms);
  }
  // Wait and check if the sensor is ready:
  this_thread::sleep_for(2000ms);
  if (allThreadsMustExit)
    throw runtime_error(
      "\n\n==== ABORTING: It seems that we could not connect to the "
      "LIDAR. See reported errors. ==== \n");

  // Load config from file:
  const string OUT_DIR_STD = iniFile.read_string(
    "MappingApplication", "logOutput_dir", "log_out",
    /*Force existence:*/ true);
  const int LOG_FREQUENCY = iniFile.read_int(
    "MappingApplication", "LOG_FREQUENCY", 5, /*Force existence:*/ true);
  const bool SAVE_IMAGE = iniFile.read_bool(
    "MappingApplication", "SAVE_IMAGE", false, /*Force existence:*/ true);

  const char* OUT_DIR = OUT_DIR_STD.c_str();

  // Constructor of ICP-SLAM object
  CMetricMapBuilderICP mapBuilder;

  mapBuilder.ICP_options.loadFromConfigFile(iniFile, "MappingApplication");
  mapBuilder.ICP_params.loadFromConfigFile(iniFile, "ICP");

  // Construct the maps with the loaded configuration.
  mapBuilder.initialize();

  // CMetricMapBuilder::TOptions
  mapBuilder.setVerbosityLevel(mrpt::system::LVL_ERROR);

  mapBuilder.ICP_params.dumpToConsole();
  mapBuilder.ICP_options.dumpToConsole();

  // Checks:
  mrpt::system::CTicTac tictac, tictacGlobal, tictac_JH;
  int step = 0;
  string str;
  float t_exec;

  // Prepare output directory:
  mrpt::system::deleteFilesInDirectory(OUT_DIR);
  mrpt::system::createDirectory(OUT_DIR);

  // Open log files:
  CFileOutputStream f_path(format("%s/log_estimated_path.txt", OUT_DIR));

  // Map Building
  tictacGlobal.Tic();
  mrpt::system::CTicTac timeout_read_scans;
  while (!allThreadsMustExit) {
    // Check for exit app:
    if (mrpt::system::os::kbhit()) {
      const char c = mrpt::system::os::getch();
      if (c == 27) break;
    }

    // Load sensor LIDAR data from live capture:
    CObservation2DRangeScan::Ptr observation;
    {
      mrpt::hwdrivers::CGenericSensor::TListObservations obs_copy;
      {
        lock_guard<mutex> csl(cs_global_list_obs);
        obs_copy = global_list_obs;
        global_list_obs.clear();
      }
      // Keep the most recent laser scan:
      for (mrpt::hwdrivers::CGenericSensor::TListObservations::
           reverse_iterator it = obs_copy.rbegin();
         !observation && it != obs_copy.rend(); ++it)
        if (it->second && IS_CLASS(it->second, CObservation2DRangeScan))
          observation =
            dynamic_pointer_cast<CObservation2DRangeScan>(
              it->second);
    }

    // If we don't have a laser scan, wait for one:
    if (!observation) {
      if (timeout_read_scans.Tac() > 1.0) {
        timeout_read_scans.Tic();
        cout << "[Warning] *** Waiting for laser scans from the Device "
            "***\n";
      }
      this_thread::sleep_for(1ms);
      continue;
    } else {
      timeout_read_scans.Tic();  // Reset timeout
    }

    // Execute ICP-SLAM:
    tictac.Tic();
    mapBuilder.processObservation(observation);
    t_exec = tictac.Tac();
    printf("Map building executed in %.03fms\n", 1000.0f * t_exec);

    // Info log:
    const CMultiMetricMap* mostLikMap = mapBuilder.getCurrentlyBuiltMetricMap();

    CPose3D robotPose;
    mapBuilder.getCurrentPoseEstimation()->getMean(robotPose);

    cout << "x: " << robotPose.x() << ", " <<
            "y: " << robotPose.y() << ", " <<
            "angle: " << RAD2DEG(robotPose.yaw()) << endl;

    // Send to port via UDP
    client.send(json_message(robotPose.x(), robotPose.y(), RAD2DEG(robotPose.yaw())));
    
    if ((step % LOG_FREQUENCY) == 0 && SAVE_IMAGE) {
      CSimpleMap map;
      mapBuilder.getCurrentlyBuiltMap(map);

      COccupancyGridMap2D gridMap;
      gridMap.loadFromSimpleMap(map);

      gridMap.saveAsBitmapFile("../server/map.bmp");
    }

    // Save the robot estimated pose for each step:
    f_path.printf(
      "%i %f %f %f\n", step,
      mapBuilder.getCurrentPoseEstimation()->getMeanVal().x(),
      mapBuilder.getCurrentPoseEstimation()->getMeanVal().y(),
      mapBuilder.getCurrentPoseEstimation()->getMeanVal().yaw());

    step++;
    printf(
      "\n---------------- STEP %u ----------------\n", step);
  };

  printf(
    "\n---------------- END!! (total time: %.03f sec) ----------------\n",
    tictacGlobal.Tac());

  // Save map:
  CSimpleMap finalMap;
  mapBuilder.getCurrentlyBuiltMap(finalMap);

  str = format("%s/_finalmap_.simplemap", OUT_DIR);
  printf("Dumping final map in binary format to: %s\n", str.c_str());
  mapBuilder.saveCurrentMapToFile(str);

  const CMultiMetricMap* finalPointsMap =
    mapBuilder.getCurrentlyBuiltMetricMap();
  str = format("%s/_finalmaps_.txt", OUT_DIR);
  printf("Dumping final metric maps to %s_XXX\n", str.c_str());
  finalPointsMap->saveMetricMapRepresentationToFile(str);

  cout << "Waiting for sensor thread to exit...\n";
  allThreadsMustExit = true;
  hSensorThread.join();
  cout << "Sensor thread is closed. Bye bye!\n";

  MRPT_END
}
