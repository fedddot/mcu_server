add_library(clonable_manager_wrapper INTERFACE)
target_include_directories(clonable_manager_wrapper INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    clonable_manager_wrapper 
    INTERFACE
    data_lib
    manager
    resources_vendor
    server_types
)