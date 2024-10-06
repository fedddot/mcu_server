add_library(vendor INTERFACE)
target_include_directories(vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    vendor
    INTERFACE
    request
    resource
    response
    data_lib
)

add_subdirectory(tests EXCLUDE_FROM_ALL)