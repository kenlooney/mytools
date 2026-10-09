execute_process(
    COMMAND "${CURSOR_EXECUTABLE}" "${INPUT_FILE}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error_output
    TIMEOUT 5
)

if(NOT "${result}" STREQUAL "0")
    message(FATAL_ERROR "Cursor test failed: ${result}\n${output}\n${error_output}")
endif()

set(expected_output "1:1 byte 97\n1:2 byte 10\n2:1 byte 98\n2:2 byte 10\n")
string(REPLACE "\r\n" "\n" output "${output}")
if(NOT "${output}" STREQUAL "${expected_output}")
    message(FATAL_ERROR "Unexpected cursor output.\nExpected:\n${expected_output}\nActual:\n${output}")
endif()
if(NOT "${error_output}" STREQUAL "")
    message(FATAL_ERROR "Unexpected stderr:\n${error_output}")
endif()
