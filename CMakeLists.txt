PROJECT(dunk)

CMAKE_MINIMUM_REQUIRED(VERSION 2.7)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)  # Required by CMake 2.7+
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

FIND_PACKAGE(mrpt-hwdrivers REQUIRED)
FIND_PACKAGE(mrpt-obs REQUIRED)
FIND_PACKAGE(mrpt-slam REQUIRED)
FIND_PACKAGE(mrpt-config REQUIRED)
FIND_PACKAGE(mrpt-system REQUIRED)
FIND_PACKAGE(mrpt-io REQUIRED)
FIND_PACKAGE(mrpt-opengl REQUIRED)
FIND_PACKAGE(mrpt-gui REQUIRED)

# Declare the target (an executable)
ADD_EXECUTABLE(dunk
        slam.cpp
        json.cpp
        UDPClient.cpp)
TARGET_LINK_LIBRARIES(dunk ${MRPT_LIBRARIES})

# Set optimized building:
IF(CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF()
