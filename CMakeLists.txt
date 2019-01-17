PROJECT(dunk)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)  # Required by CMake 2.7+
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

FIND_PACKAGE(MRPT 1.9.9 COMPONENTS base hwdrivers obs slam config system io gui)

# Declare the target (an executable)
ADD_EXECUTABLE(dunk
  slam.cpp
  json.cpp
  UDPClient.cpp)
TARGET_LINK_LIBRARIES(dunk
  mrpt::hwdrivers)

# Set optimized building:
IF(CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF()
