# add_library(test_case INTERFACE)
# target_sources(test_case INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/test_case.cpp)
# target_include_directories(test_case INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# target_link_libraries(
# 	test_case
# 	INTERFACE
# 	data_lib

# 	gpio
# 	test_gpi
# 	test_gpo

# 	json_data_parser
# 	json_data_serializer

# 	server
# 	server_creator
# 	server_parser
# 	server_serializer
# )

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
	server_creator
	server_parser
	server_serializer

	json_data_parser
	json_data_serializer

	gtest
	gmock
	-g
	-O0
)