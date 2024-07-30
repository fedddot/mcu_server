add_library(mcu_server_fixture INTERFACE)
target_include_directories(mcu_server_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	mcu_server_fixture
	INTERFACE
    mcu_factory_fixture
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
    buffered_message_receiver
    custom_creator
    custom_sender
    custom_task
    data_lib
    json_data_parser
    json_data_serializer
    mcu_server
    task

	gtest
)