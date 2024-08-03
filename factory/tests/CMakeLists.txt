add_library(mcu_factory_fixture INTERFACE)
target_include_directories(mcu_factory_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	mcu_factory_fixture
	INTERFACE
    data_lib
    custom_creator
    default_mcu_factory
    default_mcu_factory_parsers
    gpio
    mcu_factory
    object
    platform
    test_platform
)

add_executable(
	factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_mcu_factory.cpp
)

target_link_libraries(
	factory_tests
	PRIVATE
    data
    gpio
    inventory
    mcu_factory
    mcu_factory_fixture

	gtest
)