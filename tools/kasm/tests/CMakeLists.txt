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
