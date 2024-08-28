add_library(factory_fixture INTERFACE)
target_include_directories(factory_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	factory_fixture
	INTERFACE
    gtest
)

add_executable(
	factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_factory.cpp
)

target_link_libraries(
	factory_tests
	PRIVATE
    factory_fixture
    factory
	gtest
)