set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

cmake_minimum_required(VERSION 3.10)

# Project name and language
project(CitrusCore CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)

# add dependencies
include(cmake/CPM.cmake)

# GLM
CPMAddPackage("gh:g-truc/glm#1.0.1")

# Nameof for reflection
CPMAddPackage("gh:neargye/nameof#v0.10.4")

# JSON
CPMAddPackage(
    NAME JSON    
    GIT_REPOSITORY https://github.com/nlohmann/Json.git
    GIT_TAG v3.11.3
)

# Add the include directory as the root for the source directory
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")

# Find all source files recursively in the source directory
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

file(GLOB_RECURSE CONFIGURE_DEPENDS SOURCES
    ./source/*.cpp
    ./source/*.hssw
)

# If you're creating a library, use add_library instead of add_executable
add_library(CitrusCore SHARED ${SOURCES})

target_link_libraries(CitrusCore PRIVATE
    glm::glm
    nameof
)

target_include_directories(CitrusCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Include library headers 
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${JSON_SOURCE_DIR}/include
)

#target_include_directories(CitrusCore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)