add_library(test_platform INTERFACE)
target_include_directories(test_platform INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    test_platform
    INTERFACE
    gpio
    platform
    test_gpi
    test_gpo
)