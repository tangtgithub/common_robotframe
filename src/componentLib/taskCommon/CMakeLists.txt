
include_directories(./)


file(GLOB_RECURSE taskcommon_SRCS *.cpp)

add_library(taskcommon SHARED ${taskcommon_SRCS})


install(TARGETS taskcommon DESTINATION lib)

