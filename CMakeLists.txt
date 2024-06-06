cmake_minimum_required(VERSION 3.12)

project(cnc_control C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(parser INTERFACE)
target_include_directories(parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(serializer INTERFACE)
target_include_directories(serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(server INTERFACE)
target_include_directories(server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    server
    INTERFACE
    engine
    
    creator
    server_creator
    server_task
    data_lib
)

add_subdirectory(external)
add_subdirectory(engine)
add_subdirectory(utils)
add_subdirectory(parse)
add_subdirectory(data)
add_subdirectory(tests EXCLUDE_FROM_ALL)