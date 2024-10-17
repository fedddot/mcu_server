add_library(requests_manager INTERFACE)
target_include_directories(requests_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    requests_manager
    INTERFACE
    data_lib
    inventory
    inventory_manager
    request
    resource
    response
)

add_subdirectory(tests EXCLUDE_FROM_ALL)