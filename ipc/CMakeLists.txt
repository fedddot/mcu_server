add_library(ipc_connection INTERFACE)
target_include_directories(ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(util)