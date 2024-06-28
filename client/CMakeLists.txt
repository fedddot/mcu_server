add_library(client INTERFACE)
target_include_directories(client INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(client INTERFACE connection)

add_subdirectory(tests EXCLUDE_FROM_ALL)