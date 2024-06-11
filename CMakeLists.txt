cmake_minimum_required(VERSION 3.12)

project(mcu_control C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(listener INTERFACE)
target_include_directories(listener INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(connection INTERFACE)
target_include_directories(connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(connection INTERFACE listener)

add_subdirectory(engine)
add_subdirectory(server)
add_subdirectory(external)
add_subdirectory(utils)

add_subdirectory(tests EXCLUDE_FROM_ALL)