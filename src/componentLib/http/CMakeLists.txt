# http client
include_directories(.)
file(GLOB_RECURSE http_SRCS ./*.cpp)
add_library(httpClient SHARED ${http_SRCS})
target_link_libraries(httpClient  crypto pthread curl ssl)
install(TARGETS httpClient DESTINATION lib)

