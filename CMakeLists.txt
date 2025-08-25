set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

cmake_minimum_required(VERSION 3.10)

# Project name and language
project(CitrusCore CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 20)

# add dependencies
include(cmake/CPM.cmake)

# GLM
CPMAddPackage("gh:g-truc/glm#1.0.1")

# Add the include directory as the root for the source directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")

# Find all source files recursively in the source directory
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# Add executable or library (depending on your project type)
# add_executable(MyProject ${SOURCES})

# If you're creating a library, use add_library instead of add_executable
add_library(CitrusCore SHARED ${SOURCES})

target_link_libraries(CitrusCore PRIVATE
    glm::glm
)

target_include_directories(CitrusCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
#target_include_directories(CitrusCore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)