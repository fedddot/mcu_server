cmake_minimum_required(VERSION 3.16)

project(mcu_server C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ipc_connection INTERFACE)
target_include_directories(ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(provider INTERFACE)
target_include_directories(provider INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(host INTERFACE)
target_include_directories(host INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(host INTERFACE manager provider ipc_connection)

# add_subdirectory(manager)
# add_subdirectory(ipc)
# add_subdirectory(provider)
add_subdirectory(external)

add_subdirectory(tests EXCLUDE_FROM_ALL)