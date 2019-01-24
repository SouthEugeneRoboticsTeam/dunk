PROJECT(dunk)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

#FIND_PACKAGE(MRPT COMPONENTS hwdrivers obs slam config system io opengl gui)
FIND_PACKAGE(mrpt-hwdrivers REQUIRED)
FIND_PACKAGE(mrpt-obs REQUIRED)
FIND_PACKAGE(mrpt-slam REQUIRED)
FIND_PACKAGE(mrpt-config REQUIRED)
FIND_PACKAGE(mrpt-system REQUIRED)
FIND_PACKAGE(mrpt-io REQUIRED)
FIND_PACKAGE(mrpt-opengl REQUIRED)
FIND_PACKAGE(mrpt-gui REQUIRED)
FIND_PACKAGE(mrpt-utils REQUIRED)

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
  mrpt::opengl
  mrpt::gui
  mrpt::utils

)

message(STATUS "MRPT_VERSION: ${MRPT_VERSION}")
message(STATUS "MRPT_LIBRARIES: ${MRPT_LIBRARIES}")

# Set optimized building:
IF(CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF()
