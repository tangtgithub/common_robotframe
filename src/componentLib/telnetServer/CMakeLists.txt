file(GLOB_RECURSE telnetSer_SRCS *.cpp )
include_directories(./)
include_directories(${PROJECT_SOURCE_DIR}/src/cphu/inc)

add_library(telnetServer SHARED ${telnetSer_SRCS})
target_link_libraries(telnetServer pthread )
install(TARGETS telnetServer DESTINATION lib)

