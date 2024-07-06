cmake_minimum_required(VERSION 3.12)

project(mcu_server C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(data_sender INTERFACE)
target_include_directories(data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(data_extractor INTERFACE)
target_include_directories(data_extractor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(parser INTERFACE)
target_include_directories(parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(serializer INTERFACE)
target_include_directories(serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(mcu_server INTERFACE)
target_include_directories(mcu_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    mcu_server
    INTERFACE
    data_sender
    engine
    data_extractor
    serializer
    parser
)

add_subdirectory(external)
# add_subdirectory(utils)

add_subdirectory(tests EXCLUDE_FROM_ALL)