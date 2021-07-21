file(GLOB_RECURSE generalAbility_SRCS ./*.cpp)

include_directories(./)

add_library(generalAbility SHARED ${generalAbility_SRCS})
install(TARGETS generalAbility DESTINATION lib)
