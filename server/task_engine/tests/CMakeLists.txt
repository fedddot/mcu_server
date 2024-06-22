add_library(test_gpi INTERFACE)
target_include_directories(test_gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpi INTERFACE gpi)

add_library(test_gpo INTERFACE)
target_include_directories(test_gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpo INTERFACE gpo)

add_executable(
	server_task_engine_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_server_task_engine.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_gpio_tasks.cpp
)

target_link_libraries(
	server_task_engine_tests
	PRIVATE
	functional_parser
	test_gpi
	test_gpo
	json_data_parser
	json_data_serializer

	create_gpio_task
	delete_gpio_task
	set_gpio_task
	get_gpio_task
	
    # server_task_engine

	gtest
	gmock
	-g
	-O0
)