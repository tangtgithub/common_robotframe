file(GLOB_RECURSE mqtt_SRCS ./*.cpp)

include_directories(./)
add_library(mqttclient SHARED ${mqtt_SRCS})
target_link_libraries(mqttclient pthread mosquitto cryptopp)
install(TARGETS mqttclient DESTINATION lib)

