cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(jaytracer VERSION 1.0.0)

## Build

# Add src and header files
file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Bundle executable
set(EXECUTABLE_NAME jaytracer-core)
add_executable(${EXECUTABLE_NAME} ${HEADER_FILES} ${SOURCE_FILES})

## Packages

# OpenGL
find_package(OpenGL REQUIRED)

# glfw
add_subdirectory(vendor/glfw EXCLUDE_FROM_ALL)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(vendor/glew EXCLUDE_FROM_ALL)

# Set libraries for linker
set(VENDOR_LIBRARIES
    OpenGL::GL
    glfw
    libglew_static
)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${VENDOR_LIBRARIES})
