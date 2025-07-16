add_executable(
	thermostat_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_thermostat_host_builder.cpp
)

target_link_libraries(
	thermostat_app_tests
	PRIVATE
    ipc_data
    ipc_instance
    manager_instance
    relay_controller
    temperature_sensor_controller
    test_ipc_data_reader
    test_ipc_data_writer
    thermostat_host_builder
    timer_scheduler
	
	gtest
	gmock
)