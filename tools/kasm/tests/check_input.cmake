if(TEST_CASE STREQUAL "loads_input")
    set(arguments "${INPUT_FILE}")
    file(SIZE "${INPUT_FILE}" input_size)
    file(READ "${INPUT_FILE}" input_content)
    set(expected_output "Loaded source file: ${INPUT_FILE}\nSource length: ${input_size}\nSource content:\n${input_content}\n")
elseif(TEST_CASE STREQUAL "requires_input")
    set(arguments)
    set(expected_output "Usage: ${KASM_EXECUTABLE} <source_file>\n")
elseif(TEST_CASE STREQUAL "missing_file")
    if(EXISTS "${MISSING_FILE}")
        message(FATAL_ERROR "Missing-file test path unexpectedly exists: ${MISSING_FILE}")
    endif()
    set(arguments "${MISSING_FILE}")
    set(expected_output "Failed to load source file: ${MISSING_FILE}\n")
else()
    message(FATAL_ERROR "Unknown input test case: ${TEST_CASE}")
endif()

execute_process(
    COMMAND "${KASM_EXECUTABLE}" ${arguments}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error_output
    TIMEOUT 5
)

if(NOT "${result}" MATCHES "^-?[0-9]+$")
    message(FATAL_ERROR "kasm did not exit normally: ${result}\n${output}\n${error_output}")
endif()
if(TEST_CASE STREQUAL "loads_input")
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Expected success, got exit status ${result}\n${output}\n${error_output}")
    endif()
elseif(result EQUAL 0)
    message(FATAL_ERROR "Expected a nonzero exit status\n${output}\n${error_output}")
endif()

string(REPLACE "\r\n" "\n" output "${output}")
string(REPLACE "\r\n" "\n" expected_output "${expected_output}")
if(NOT "${output}" STREQUAL "${expected_output}")
    message(FATAL_ERROR "Unexpected output.\nExpected:\n${expected_output}\nActual:\n${output}")
endif()
if(NOT "${error_output}" STREQUAL "")
    message(FATAL_ERROR "Unexpected stderr:\n${error_output}")
endif()
