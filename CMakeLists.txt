cmake_minimum_required(VERSION 3.16)

project(mcu_server C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(app)
add_subdirectory(host)
add_subdirectory(manager)
add_subdirectory(ipc)
add_subdirectory(external)
add_subdirectory(vendor)