add_library(in_memory_inventory INTERFACE)
target_include_directories(in_memory_inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    in_memory_inventory
    INTERFACE
    inventory
)