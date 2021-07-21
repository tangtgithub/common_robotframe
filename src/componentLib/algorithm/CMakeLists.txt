
include_directories(./)

file(GLOB_RECURSE algorithm_SRCS *.cpp)

add_library(algorithm SHARED ${algorithm_SRCS})

find_package( OpenCV 3 REQUIRED )

target_link_libraries(algorithm  ${OpenCV_LIBS})

install(TARGETS algorithm DESTINATION lib)

