file(GLOB_RECURSE robot_SRCS ./*.cpp  )

include_directories(${PROJECT_SOURCE_DIR}/src/centre)


LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/database/)
add_executable(SLRobot ${robot_SRCS})
target_link_libraries(SLRobot sqlite3 centralPM  pthread mqttclient telnetServer websocketServer  navicat database httpClient  generalAbility pushRtmp taskcommon stateManager taskScheduler testRobot algorithm ${BOOSTSYSTEM_LIBRARY} boost_filesystem msc m z)


install(TARGETS SLRobot DESTINATION bin)
