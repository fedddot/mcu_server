add_library(server_fixture INTERFACE)
target_include_directories(server_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	server_fixture
	INTERFACE
    custom_creator
    data_lib
    default_gpio_tasks_data_retriever
    gpio
    gpio_tasks_factory
    gpio_test_data_creator
    test_platform
    gtest
)

add_executable(
	server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_server.cpp
)

target_link_libraries(
	server_tests
	PRIVATE
    data_lib
    gpio
    server_fixture

	gtest
)