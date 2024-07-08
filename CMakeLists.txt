cmake_minimum_required(VERSION 3.12)

project(mcu_server C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(data_sender INTERFACE)
target_include_directories(data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(listener INTERFACE)
target_include_directories(listener INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(data_receiver INTERFACE)
target_include_directories(data_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(data_receiver INTERFACE listener)

add_library(parser INTERFACE)
target_include_directories(parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(serializer INTERFACE)
target_include_directories(serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(mcu_server INTERFACE)
target_include_directories(mcu_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    mcu_server
    INTERFACE
    custom_listener
    data
    data_receiver
    data_sender
    engine
    parser
    serializer
)

add_subdirectory(external)
add_subdirectory(util)

add_subdirectory(tests EXCLUDE_FROM_ALL)