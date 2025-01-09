
add_library(test_manager INTERFACE)
target_include_directories(test_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_manager INTERFACE manager)