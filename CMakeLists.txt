cmake_minimum_required(VERSION 3.10)
project(CITS2002_Project_2 C)

set(CMAKE_C_STANDARD 99)

add_executable(mergetars mergetars.c mergetars.h run_cmd.c myfunction.c expand_tars.c compare_files.c copy_files.c create_output_tar.c cleanup.c sysbin.h)
add_executable(test_thing test_thing.c)