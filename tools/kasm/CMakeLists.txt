file(GLOB_RECURSE KASM_SOURCES CONFIGURE_DEPENDS "src/*.c")
add_executable(kasm ${KASM_SOURCES})

target_include_directories(kasm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(kasm PUBLIC common)
include(GNUInstallDirs)
install(TARGETS kasm RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
