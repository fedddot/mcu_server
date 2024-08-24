add_executable(
	gpio_tasks_factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_gpio_tasks_factory.cpp
)

target_link_libraries(
	gpio_tasks_factory_tests
	PRIVATE
    gpio_tasks_factory
	default_gpio_tasks_data_retriever
	custom_creator
	gtest
)