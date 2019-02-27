# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Project's name
project(MainProject)
set (src_dir ${CMAKE_SOURCE_DIR}/src)
set (asset_dir ${CMAKE_SOURCE_DIR}/assets)
message(${src_dir}!!!!!)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

# The following folder will be included
include_directories("${CMAKE_SOURCE_DIR}/includes")
include_directories("${CMAKE_SOURCE_DIR}/includes/soil")
include_directories("${CMAKE_SOURCE_DIR}/lib")
#link_directories(${libraries})

#set(CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")


# Laitetaan make tulostamaan tekosensa.
set(CMAKE_VERBOSE_MAKEFILE ON)

# MAKE_RUNTIME_OUTPUT_DIRECTORY or EXECUTABLE_OUTPUT_PATH. Where to create the executable. 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Library output path.
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

# The following folder will be included
add_subdirectory("${src_dir}")
