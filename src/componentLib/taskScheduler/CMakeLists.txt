file(GLOB_RECURSE taskScheduler_SRCS ./*.cpp)

include_directories(./) 

add_library(taskScheduler SHARED ${taskScheduler_SRCS})
#target_link_libraries(taskmodule pthread)
install(TARGETS taskScheduler DESTINATION lib)
