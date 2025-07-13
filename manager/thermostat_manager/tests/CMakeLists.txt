add_executable(
	thermostat_manager_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_thermostat_manager.cpp
)

target_link_libraries(
	thermostat_manager_tests
	PRIVATE
    thermostat_manager
    gtest
	gmock
)