cmake_minimum_required(VERSION 3.3)

set(SDL_SHARED 1)
add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(glfw EXCLUDE_FROM_ALL)
add_subdirectory(glm EXCLUDE_FROM_ALL)
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
add_subdirectory(glad/cmake EXCLUDE_FROM_ALL)

glad_add_library(glad_gl_core_33 STATIC API gl:core=3.3)

