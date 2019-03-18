cmake_minimum_required(VERSION 3.13)
project(lab4 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror --std=gnu99 -pedantic ")

add_executable(lab4 builtins.c builtins.h sh.c defs.h io.c line_parser.c variables.c)
add_executable(xargs xargs/xargs.c)
