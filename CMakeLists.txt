cmake_minimum_required(VERSION 2.6)

project(RobotSelf)

if((NOT DEFINED CMAKE_BUILD_TYPE) OR (NOT CMAKE_BUILD_TYPE))
  #set(CMAKE_BUILD_TYPE "Release")
  set(CMAKE_BUILD_TYPE "Debug")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
message(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
  message(STATUS "use ccache")
  SET(CMAKE_CXX_COMPILER      "ccache")
  SET(CMAKE_CXX_COMPILER_ARG1 "g++")
endif()

set(CXX_FLAGS
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -fPIC
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++14
 -rdynamic
 -Wno-error=deprecated-declarations
 -Wno-deprecated-declarations
 #-Wno-implicit-fallthrough
 )
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND CXX_FLAGS "-Wno-null-dereference")
  list(APPEND CXX_FLAGS "-Wno-sign-conversion")
  list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
  list(APPEND CXX_FLAGS "-Wthread-safety")
  list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
OPTION (ENABLE_COVERAGE "Use gcov" off)
MESSAGE(STATUS ENABLE_COVERAGE=${ENABLE_COVERAGE})
IF(ENABLE_COVERAGE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF()
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_library(BOOSTSYSTEM_LIBRARY NAMES boost_system PATHS /usr/lib/x86_64-linux-gnu/)


include_directories(${PROJECT_SOURCE_DIR}/src/globalinclude)
include_directories(${PROJECT_SOURCE_DIR}/src/globalinclude/common)
include_directories(${PROJECT_SOURCE_DIR}/src/globalinclude/sdkCallback)
include_directories(${PROJECT_SOURCE_DIR}/src/globalinclude/commonFunc)
include_directories(${PROJECT_SOURCE_DIR}/src/globalinclude/thirdparty)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

#*****选择机器人*****
if(NOT DEFINED ROBOT_TYPE)
    message(STATUS "use ROBOT_TYPE from cmakelist")
    set(ROBOT_TYPE "ROBOT_TEST")               #测试机器人
endif()
#********************


message(STATUS "ROBOT_TYPE=" ${ROBOT_TYPE})

include_directories(/usr/include/jsoncpp)

find_path(ffmpeg_path2 libavcodec.so /usr/local/ffmpeg/lib) # 针对192.168.16.250远程机器

include_directories(/usr/local/ffmpeg/include)
link_directories(/usr/local/ffmpeg/lib)



if(LOG STREQUAL "FILELOG")
  set(LOG_TYPE "FILELOG")
elseif(LOG STREQUAL "STDLOG")
  set(LOG_TYPE "STDLOG")
else()
  set(LOG_TYPE "FILELOG")
endif()

configure_file (
    ${PROJECT_SOURCE_DIR}/robotConfig.h.in
    ${PROJECT_SOURCE_DIR}/src/globalinclude/common/robotConfig.h)

link_directories(/usr/local/lib)
if(${ROBOT_TYPE} STREQUAL "ROBOT_TEST")
add_subdirectory(src/centre)
add_subdirectory(src/componentLib/websocketServer)
add_subdirectory(src/componentLib/database)
add_subdirectory(src/componentLib/navicat)
add_subdirectory(src/componentLib/http)
add_subdirectory(src/componentLib/telnetServer)
add_subdirectory(src/componentLib/mqttClient)
add_subdirectory(src/componentLib/pushRtmp)
add_subdirectory(src/componentLib/generalAbility)
add_subdirectory(src/componentLib/algorithm)
add_subdirectory(src/componentLib/stateManager)
add_subdirectory(src/componentLib/taskCommon)
add_subdirectory(src/componentLib/taskScheduler)
add_subdirectory(src/robot/test_robot)
add_subdirectory(src/main)

elseif(${ROBOT_TYPE} STREQUAL "ROBOT_OTHER")

else(${ROBOT_TYPE} STREQUAL "UNIT_TEST")

endif()
