add_executable(
	util_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_data_parser.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_data_serializer.cpp
)

target_link_libraries(
	util_tests
	PRIVATE
	json_data_parser
	json_data_serializer
	data_lib
	
	gtest
)
