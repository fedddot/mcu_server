add_executable(
	engine_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_engine.cpp
)

target_link_libraries(
	engine_tests
	PRIVATE
	engine
	functional_creator
	functional_parser
	functional_serializer
	functional_task

	gtest
	gmock
	-g
	-O0
)