cmake_minimum_required(VERSION 3.12)

project(mcu_server C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(request INTERFACE)
target_include_directories(request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(request INTERFACE data_lib)

add_library(response INTERFACE)
target_include_directories(response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(response INTERFACE data_lib)

add_library(resource INTERFACE)
target_include_directories(resource INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(resource INTERFACE request response)

add_subdirectory(data)
add_subdirectory(external)
add_subdirectory(ipc)
add_subdirectory(manager)
add_subdirectory(vendor)

add_subdirectory(tests EXCLUDE_FROM_ALL)