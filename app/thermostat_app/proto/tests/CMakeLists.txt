add_executable(
	thermostat_proto_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_proto_thermostat_api_request_parser.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_proto_thermostat_api_response_serializer.cpp
)

target_link_libraries(
	thermostat_proto_tests
	PRIVATE
    proto_thermostat_api_request_parser
	proto_thermostat_api_response_serializer
	
	pb_encode
	gtest
)