add_library(gpio_tasks_factory_fixture INTERFACE)
target_include_directories(gpio_tasks_factory_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	gpio_tasks_factory_fixture
	INTERFACE
    custom_creator
    data_lib
    default_gpio_tasks_data_retriever
    gpio
    gpio_tasks_factory
    test_platform
)

add_executable(
	gpio_tasks_factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_gpio_tasks_factory.cpp
)

target_link_libraries(
	gpio_tasks_factory_tests
	PRIVATE
    gpio_tasks_factory
	gpio_tasks_factory_fixture
	gtest
)