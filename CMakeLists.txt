cmake_minimum_required(VERSION 3.16)
project(GFX
  VERSION 1.0.0
  DESCRIPTION "A dumb-as-nails graphics framework build on SDL2")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(libs)

set(GFX_SOURCES
  src/graphics.cpp
  src/window.cpp
  src/shader.cpp
  src/mesh.cpp
)

set(GFX_LIBS
  glm::glm
  glad_gl_core_33
  spdlog::spdlog
)

add_library(gfx-sdl ${GFX_SOURCES})
target_compile_definitions(gfx-sdl PRIVATE USE_SDL)
target_include_directories(gfx-sdl PUBLIC include)
target_link_libraries(gfx-sdl PRIVATE SDL2 ${GFX_LIBS})

add_library(gfx-glfw ${GFX_SOURCES})
target_include_directories(gfx-glfw PUBLIC include)
target_link_libraries(gfx-glfw PRIVATE glfw ${GFX_LIBS})

option(BUILD_DOC "Build documentation" OFF)
find_package(Doxygen)
if (BUILD_DOC AND DOXYGEN_FOUND)
  # Configure Doxyfile
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  add_custom_target(docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM)
elseif(BUILD_DOC)
  message("Doxygen needs to be installed to generate the documentation")
endif()
