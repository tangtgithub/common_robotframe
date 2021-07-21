
include_directories(./)

if(${ROBOT_TYPE} STREQUAL "THERMAL_DETECTOR" OR ${ROBOT_TYPE} STREQUAL "CLEANING_KING" OR ${ROBOT_TYPE} STREQUAL "VEHICLE")

	file(GLOB_RECURSE pushRtmp_SRCS *.cpp)

	add_library(pushRtmp OBJECT ${pushRtmp_SRCS})

elseif(${ROBOT_TYPE} STREQUAL "VIDEO_STREAM_ALWAYS")

	file(GLOB_RECURSE pushRtmp_SRCS *.cpp ./public/videoStream.cpp)

	add_library(pushRtmp SHARED ${pushRtmp_SRCS})

	target_link_libraries(pushRtmp avformat avcodec avutil pthread)

	install(TARGETS pushRtmp DESTINATION lib)

elseif(${ROBOT_TYPE} STREQUAL "VIDEO_STREAM_ALWAYS_SWITCH")

	file(GLOB_RECURSE pushRtmp_SRCS *.cpp ./public/VideoStreamSwitch.cpp)
	add_library(pushRtmp SHARED ${pushRtmp_SRCS})
	target_link_libraries(pushRtmp avformat avcodec avutil x264 pthread)
	install(TARGETS pushRtmp DESTINATION lib)
	# file(GLOB_RECURSE pushRtmp_SRCS *.cpp ./public/VideoStreamSwitch.cpp ./public/PushMain.cpp ../globalinclude/thirdparty/xml2/*.cpp)
	# add_executable(pushRtmp ${pushRtmp_SRCS})
	# target_link_libraries(pushRtmp avformat avcodec avutil swscale opencv_videoio opencv_core opencv_highgui pthread)
	# install(TARGETS pushRtmp DESTINATION bin)

else()

	file(GLOB_RECURSE pushRtmp_SRCS *.cpp)

	add_library(pushRtmp SHARED ${pushRtmp_SRCS})

	target_link_libraries(pushRtmp avformat avcodec avutil pthread)

	install(TARGETS pushRtmp DESTINATION lib)

endif()