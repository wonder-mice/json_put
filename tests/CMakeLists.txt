cmake_minimum_required(VERSION 3.2)

include(CMakeParseArguments)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -pedantic-errors")

add_executable(output_tests output_tests.c)
target_link_libraries(output_tests json_put)
add_test(NAME output_tests COMMAND output_tests)
