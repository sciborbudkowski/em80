cmake_minimum_required(VERSION 3.31)
project(em80)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(raylib REQUIRED)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h)

set(MOC_HEADERS "")
foreach(_header ${HEADER_FILES})
    file(READ ${_header} _content)
    if("${_content}" MATCHES "Q_OBJECT")
        list(APPEND MOC_HEADERS ${_header})
    endif()
endforeach(_header ${HEADER_FILES})
qt6_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

qt_add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_HEADERS}
    ${MOC_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/emulator
    ${CMAKE_SOURCE_DIR}/include/emulator/base
    ${CMAKE_SOURCE_DIR}/include/emulator/cpu
    ${CMAKE_SOURCE_DIR}/include/emulator/memory
    ${CMAKE_SOURCE_DIR}/include/emulator/io
    ${CMAKE_SOURCE_DIR}/include/views
    ${SDL2_INCLUDE_DIRS}
    ${Qt6_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    nlohmann_json::nlohmann_json
    raylib
)
