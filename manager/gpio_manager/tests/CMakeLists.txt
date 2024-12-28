add_library(test_gpi INTERFACE)
target_include_directories(test_gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpi INTERFACE gpi)

add_library(test_gpo INTERFACE)
target_include_directories(test_gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpo INTERFACE gpo)

add_executable(
	gpio_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_gpio_manager.cpp
)

target_link_libraries(
	gpio_manager_tests
	PRIVATE
    gpio
    gpio_create_request
    gpio_manager
    gpio_provider
    gpio_regular_response
    gpio_request
    gpio_response
    gpio_state_response
    test_gpi
    test_gpo
	gtest
)