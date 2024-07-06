cmake_minimum_required(VERSION 3.12)

project(mcu_server C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(retriever INTERFACE)
target_include_directories(retriever INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(mcu_server INTERFACE)
target_include_directories(mcu_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    mcu_server
    INTERFACE
)

add_subdirectory(external)
# add_subdirectory(utils)

# add_subdirectory(tests EXCLUDE_FROM_ALL)