add_executable(
	thermostat_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_thermostat_host_builder.cpp
)

target_link_libraries(
	thermostat_app_tests
	PRIVATE
	thermostat_host_builder
	thermostat_controller
	thermostat_service
	
	gtest
	gmock
)