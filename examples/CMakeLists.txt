 cmake_minimum_required(VERSION 3.2)

 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -pedantic-errors")

 add_executable(hello_c hello.c)
 target_link_libraries(hello_c json_put)

 add_executable(hello_cpp hello.cpp)
 target_link_libraries(hello_cpp json_put)
