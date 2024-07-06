add_library(mcu_server_fixture INTERFACE)
target_include_directories(mcu_server_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	mcu_server_fixture
	INTERFACE
    mcu_task_engine_fixture
)

add_executable(
	mcu_server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_mcu_server.cpp
)

target_link_libraries(
	mcu_server_tests
	PRIVATE
	mcu_server_fixture
	mcu_server

	gtest
)