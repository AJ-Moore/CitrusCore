set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.10)

# Project name and language
project(CitrusCore CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Add the include directory as the root for the source directory
include_directories("${CMAKE_SOURCE_DIR}/source")

# Find all source files recursively in the source directory
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/source/*.cpp")

# Add executable or library (depending on your project type)
# add_executable(MyProject ${SOURCES})

# If you're creating a library, use add_library instead of add_executable
add_library(CitrusCore ${SOURCES})
