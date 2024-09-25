cmake_minimum_required(VERSION 3.12)

project(mcu_server C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(request INTERFACE)
target_include_directories(request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    request
    INTERFACE
    object
)

add_library(resource INTERFACE)
target_include_directories(resource INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)


add_library(task INTERFACE)
target_include_directories(task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(server INTERFACE)
target_include_directories(server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    server
    INTERFACE
    creator
    data
    task
)

add_subdirectory(data)
add_subdirectory(external)
add_subdirectory(factory)
add_subdirectory(ipc)
add_subdirectory(platform)
add_subdirectory(util)

add_subdirectory(tests EXCLUDE_FROM_ALL)