PROJECT(dunk)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

FIND_PACKAGE(MRPT COMPONENTS hwdrivers obs slam config system io opengl gui) 

message(STATUS "MRPT_VERSION: ${MRPT_VERSION}")
message(STATUS "MRPT_LIBRARIES: ${MRPT_LIBRARIES}")

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
