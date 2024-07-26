cmake_minimum_required(VERSION 3.12)

project(mcu_server C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(creator INTERFACE)
target_include_directories(creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(parser INTERFACE)
target_include_directories(parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(serializer INTERFACE)
target_include_directories(serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(task INTERFACE)
target_include_directories(task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(mcu_server INTERFACE)
target_include_directories(mcu_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    mcu_server
    INTERFACE
    creator
    data
    message_receiver
    message_sender
    parser
    serializer
    task
)

add_subdirectory(data)
add_subdirectory(external)
add_subdirectory(platform)
add_subdirectory(util)

add_subdirectory(tests EXCLUDE_FROM_ALL)