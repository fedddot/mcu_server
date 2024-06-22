add_executable(
	server_task_engine_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_server_task_engine.cpp
)

target_link_libraries(
	server_task_engine_tests
	PRIVATE
    server_task_engine

	gtest
	gmock
	-g
	-O0
)