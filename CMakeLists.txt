cmake_minimum_required (VERSION 2.6)
project(pam_openam)
add_executable(test src/pam_openam.c src/commons.c)
target_link_libraries(test curl)
