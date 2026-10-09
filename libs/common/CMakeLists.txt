file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
add_library(common ${SOURCES})
target_include_directories(common PUBLIC ${CMAKE_SOURCE_DIR}/include)