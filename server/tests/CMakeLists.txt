add_library(test_gpo INTERFACE)
target_include_directories(test_gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpo INTERFACE gpo)

add_library(test_gpi INTERFACE)
target_include_directories(test_gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpi INTERFACE gpi)

add_executable(
	server_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_server.cpp
)

target_link_libraries(
	server_tests
	PRIVATE
    server

    data_lib

    functional_connection
    functional_creator
    functional_parser
	json_data_parser
    functional_serializer
	json_data_serializer
    functional_task
	test_gpo
	test_gpi
	gtest
	gmock
	-g
	-O0
)