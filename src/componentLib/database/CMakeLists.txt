

include_directories(./)
include_directories(
	${CMAKE_SOURCE_DIR}
	)

file(GLOB_RECURSE database_SRCS ./*.cpp)
add_library(database SHARED ${database_SRCS})
target_link_libraries(
	database
	sqlite3
    pthread
	)
install(TARGETS database DESTINATION lib)
