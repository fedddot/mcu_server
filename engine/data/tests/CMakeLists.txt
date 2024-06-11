add_executable(
	data_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_data.cpp
)

target_link_libraries(
	data_tests
	PRIVATE
	data_lib
	
	gtest
	gmock
	-g
	-O0
)
