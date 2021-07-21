#很多系统没有完整包，使用find_package可能失败
#find_library(BOOSTSYSTEM_LIBRARY NAMES boost_system)
find_library(Boost_LIBRARIES NAMES boost_system)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# websocket server
file(GLOB_RECURSE websocket_SRCS ./*.cpp)

add_library(websocketServer SHARED ${websocket_SRCS})

# link
target_link_libraries(websocketServer ${Boost_LIBRARIES} jsoncpp)

# install
install(TARGETS websocketServer DESTINATION lib)
