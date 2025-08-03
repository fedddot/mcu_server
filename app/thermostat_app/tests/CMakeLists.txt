add_executable(
	thermostat_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_thermostat_app.cpp
)

target_link_libraries(
	thermostat_app_tests
	PRIVATE
    ipc_queue
    package_utils
    proto_thermostat_api_request_parser
    proto_thermostat_api_request_serializer
    proto_thermostat_api_response_serializer
    ring_queue
    thermostat_app
    thermostat_controller
	
	gtest
	gmock
)