add_library(test_task_creator INTERFACE)
target_include_directories(test_task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	test_task_creator
	INTERFACE
    creator
    data
    task
    task_creator
)

add_executable(
	factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_factory.cpp
)

target_link_libraries(
	factory_tests
	PRIVATE
    custom_task
    data_lib
    factory
    task
    test_task_creator
)