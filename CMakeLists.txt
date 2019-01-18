PROJECT(dunk)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)  # Required by CMake 2.7+
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

FIND_PACKAGE(mrpt-hwdrivers)
FIND_PACKAGE(mrpt-obs)
FIND_PACKAGE(mrpt-slam)
FIND_PACKAGE(mrpt-config)
FIND_PACKAGE(mrpt-io)
FIND_PACKAGE(mrpt-gui)

# Declare the target (an executable)
ADD_EXECUTABLE(dunk
  slam.cpp
  json.cpp
  UDPClient.cpp)
TARGET_LINK_LIBRARIES(dunk
  mrpt::hwdrivers
  mrpt::obs
  mrpt::slam
  mrpt::config
  mrpt::system
  mrpt::io
  mrpt::gui)

# Set optimized building:
IF(CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF()
