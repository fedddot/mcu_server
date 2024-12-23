add_executable(
	ipc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_http_ipc_connection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_package_reader.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_package_writer.cpp
)

target_link_libraries(
	ipc_tests
	PRIVATE
    package_reader
	package_writer
	custom_data_writer
	http_ipc_connection
    ipc_types

    http_ipc_connection
    json_data_serializer
    request
    response
    server_types
    data_lib
	
	gtest
)