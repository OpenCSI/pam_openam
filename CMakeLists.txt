cmake_minimum_required (VERSION 2.6)
project(pam_openam)
add_executable(openam_validatetoken src/openam_validatetoken.c src/commons.c)
target_link_libraries(openam_validatetoken curl)
