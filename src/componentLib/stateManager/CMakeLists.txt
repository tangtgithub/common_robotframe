file(GLOB_RECURSE stateManager_SRCS ./*.cpp)

include_directories(./)

add_library(stateManager SHARED ${stateManager_SRCS})
install(TARGETS stateManager  DESTINATION lib)
