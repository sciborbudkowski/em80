cmake_minimum_required(VERSION 3.16)
project(em80)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Verbose output
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Enable verbose output" FORCE)
set(CMAKE_COLOR_MAKEFILE ON CACHE BOOL "Enable color output" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Precompiled headers
set(CMAKE_USE_PCH ON)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=5")
    set(CMAKE_CXX_COMPILER "${CMAKE_SOURCE_DIR}/scripts/cxx-wrapper.sh")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=5")
    set(CMAKE_CXX_COMPILER "${CMAKE_SOURCE_DIR}/scripts/clang-wrapper.sh")
endif()

# These options should be used for release builds
# add_compile_options(-O2 -Wall -Wextra -Wpedantic)
add_compile_options(-O2)

set(CMAKE_AUTOMOC ON)

# Turn Vulkan off
set(RALIB_MODULE_VULKAN OFF CACHE BOOL "Disable Vulkan support" FORCE)

# Find dependencies
find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

# Find headers and sources
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h)

set(INCLUDE_DIRS "")
foreach(header_file ${HEADER_FILES})
    get_filename_component(dir ${header_file} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

message(STATUS "Znalezione pliki źródłowe: ${SOURCE_FILES}")
message(STATUS "Znalezione pliki nagłówkowe: ${HEADER_FILES}")
message(STATUS "Znalezione katalogi include: ${INCLUDE_DIRS}")

qt_add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_HEADERS}
    ${MOC_SOURCES}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wno-deprecated-declarations
)

target_include_directories(${PROJECT_NAME} PRIVATE
#    ${CMAKE_SOURCE_DIR}/include
    ${INCLUDE_DIRS}
    ${RAYLIB_INCLUDE_DIRS}
    ${Qt6_INCLUDE_DIRS}
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${RAYLIB_LIBRARY_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    nlohmann_json::nlohmann_json
    ${RAYLIB_LIBRARIES}
)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
    )
endif()
