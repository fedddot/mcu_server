add_executable(
	utils_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_server_factory.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_server_inventory.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_server_parser.cpp
)

target_link_libraries(
	utils_tests
	PRIVATE
	server_factory
	server_inventory
	server_parser

	gtest
	gmock
	-g
	-O0
)