
include_directories(./)
include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE testRobot_SRCS *.cpp)

add_library(testRobot SHARED ${testRobot_SRCS})

target_link_libraries(testRobot pushRtmp avformat avcodec avutil swscale pthread)

install(TARGETS testRobot DESTINATION lib)

