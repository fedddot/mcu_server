add_library(server_fixture INTERFACE)
target_include_directories(server_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	server_fixture
	INTERFACE
    creator
    custom_creator
    data_lib
    json_data_parser
    json_data_serializer
    mcu_factory_fixture
    server
    parser
    serializer
    task
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