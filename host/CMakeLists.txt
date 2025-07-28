add_library(host INTERFACE)
target_include_directories(host INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    host
    INTERFACE
    service
)

add_subdirectory(tools)
add_subdirectory(tests EXCLUDE_FROM_ALL)