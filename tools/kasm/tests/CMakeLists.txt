configure_file(fixtures/input.asm "${CMAKE_CURRENT_BINARY_DIR}/input file.asm" COPYONLY)

foreach(case IN ITEMS loads_input requires_input missing_file)
    add_test(
        NAME kasm.${case}
        COMMAND ${CMAKE_COMMAND}
            "-DKASM_EXECUTABLE=$<TARGET_FILE:kasm>"
            "-DTEST_CASE=${case}"
            "-DINPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/input file.asm"
            "-DMISSING_FILE=${CMAKE_CURRENT_BINARY_DIR}/nonexistent input.asm"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/check_input.cmake"
    )
    set_tests_properties(kasm.${case} PROPERTIES
        LABELS "kasm;cli;input"
        TIMEOUT 10
    )
endforeach()

configure_file(
    fixtures/cursor.asm
    "${CMAKE_CURRENT_BINARY_DIR}/cursor.asm"
    @ONLY
    NEWLINE_STYLE LF
)

add_executable(kasm_cursor_test cursor_test.c)
target_link_libraries(kasm_cursor_test PRIVATE common)
add_test(
    NAME kasm.cursor
    COMMAND ${CMAKE_COMMAND}
        "-DCURSOR_EXECUTABLE=$<TARGET_FILE:kasm_cursor_test>"
        "-DINPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/cursor.asm"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/check_cursor.cmake"
)
set_tests_properties(kasm.cursor PROPERTIES
    LABELS "kasm;cli;input;cursor"
    TIMEOUT 10
)
