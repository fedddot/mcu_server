add_library(ring_queue INTERFACE)
target_include_directories(ring_queue INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(ring_queue INTERFACE ipc_queue)

add_subdirectory(tests EXCLUDE_FROM_ALL)