cmake_minimum_required(VERSION 3.28)
project(imgops C)

set(CMAKE_C_STANDARD 11)

add_library(imgops STATIC library.c)

install(TARGETS imgops DESTINATION .)

target_include_directories(imgops PUBLIC include)