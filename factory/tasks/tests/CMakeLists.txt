add_library(tasks_fixture INTERFACE)
target_include_directories(tasks_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(tasks_fixture INTERFACE mcu_factory_fixture)

add_executable(
	tasks_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_persistent_task.cpp
)

target_link_libraries(
	tasks_tests
	PRIVATE
    create_persistent_task
    delete_persistent_task
    execute_persistent_task
    execute_persistent_tasks
    inventory
    tasks_fixture
    
	gtest
)