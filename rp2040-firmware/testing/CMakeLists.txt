cmake_minimum_required(VERSION 3.0)
project(ng_test_mem)

set(SOURCES ../source/ng_mem.c)

add_executable(ng_test_mem ${SOURCES})
target_compile_definitions(ng_test_mem PRIVATE TESTING)