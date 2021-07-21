file(GLOB_RECURSE robot_SRCS ./*.cpp )

include_directories(./)

add_library(centralPM SHARED ${robot_SRCS})
target_link_libraries(centralPM pthread)

