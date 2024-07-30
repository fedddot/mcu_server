add_library(test_gpi INTERFACE)
target_include_directories(test_gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpi INTERFACE gpi)

add_library(test_gpo INTERFACE)
target_include_directories(test_gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpo INTERFACE gpo)

add_library(test_platform INTERFACE)
target_include_directories(test_platform INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    test_platform
    INTERFACE
    buffered_message_receiver
    custom_sender
    gpio
    platform
    test_gpi
    test_gpo
)