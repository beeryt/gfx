cmake_minimum_required(VERSION 3.16)
project(GFX
  VERSION 1.0.0
  DESCRIPTION "A dumb-as-nails graphics framework build on SDL2")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(gfx
  src/graphics.cpp
)
target_include_directories(gfx PUBLIC include)
target_include_directories(gfx PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(gfx PRIVATE SDL2 ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

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
