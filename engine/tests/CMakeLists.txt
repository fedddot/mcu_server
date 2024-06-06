add_library(test_creator INTERFACE)
target_include_directories(test_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_directories(test_creator INTERFACE creator)

add_library(test_engine_factory INTERFACE)
target_include_directories(test_engine_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_directories(test_engine_factory INTERFACE creator task)

add_library(test_parser INTERFACE)
target_include_directories(test_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_directories(test_parser INTERFACE parser)

add_library(test_engine_task INTERFACE)
target_include_directories(test_engine_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_directories(test_engine_task INTERFACE task)

add_executable(
	engine_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_engine.cpp
)

target_link_libraries(
	engine_tests
	PRIVATE
	engine
	test_engine_factory
	test_creator
	test_engine_task
	test_parser

	gtest
	gmock
	-g
	-O0
)