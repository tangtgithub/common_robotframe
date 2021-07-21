file(GLOB_RECURSE navicat_SRCS ./*.cpp)

add_library(navicat SHARED ${navicat_SRCS})
target_link_libraries(navicat pthread protoc)
install(TARGETS navicat DESTINATION lib)

