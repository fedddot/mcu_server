add_executable(
	thermostat_service_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_thermostat_service.cpp
)

target_link_libraries(
	thermostat_service_tests
	PRIVATE
    thermostat_manager
    thermostat_service
	
    gtest
)