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