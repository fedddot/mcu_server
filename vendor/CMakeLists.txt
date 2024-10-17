add_library(resources_vendor INTERFACE)
target_include_directories(resources_vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    resources_vendor
    INTERFACE
    data_lib
    manager
    request
    response
    vendor
)

add_subdirectory(tests EXCLUDE_FROM_ALL)