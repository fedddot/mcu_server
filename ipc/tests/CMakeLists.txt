add_library(test_ipc_data_reader INTERFACE)
target_include_directories(test_ipc_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_ipc_data_reader INTERFACE ipc_data_reader ipc_instance ipc_instance)

add_library(test_ipc_data_writer INTERFACE)
target_include_directories(test_ipc_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_ipc_data_writer INTERFACE ipc_data_writer)

add_executable(
	ipc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_api_request_reader_builder.cpp
)

target_link_libraries(
	ipc_tests
	PRIVATE
    api_request_reader_builder

	jsoncpp_static
	gtest
)