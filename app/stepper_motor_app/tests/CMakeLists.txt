set(STEPPER_SERVICE_PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/../src)
set(STEPPER_SERVICE_PROTO_NAME stepper_service.proto)
set(GENERATED_PROTO_PATH ${CMAKE_BINARY_DIR}/proto_generated_google)
add_custom_command(
	OUTPUT ${GENERATED_PROTO_PATH}/stepper_service.pb.h ${GENERATED_PROTO_PATH}/stepper_service.pb.c
	COMMAND mkdir -p ${GENERATED_PROTO_PATH} && protoc -I ${STEPPER_SERVICE_PROTO_PATH} --cpp_out=${GENERATED_PROTO_PATH} ${STEPPER_SERVICE_PROTO_NAME}
	DEPENDS ${STEPPER_SERVICE_PROTO_PATH}/${STEPPER_SERVICE_PROTO_NAME}
)

add_custom_target(
	generate_stepper_service_google
	DEPENDS ${GENERATED_PROTO_PATH}/stepper_service.pb.h ${GENERATED_PROTO_PATH}/stepper_service.pb.c
)

add_library(stepper_service_google STATIC)
target_sources(stepper_service_google PRIVATE ${GENERATED_PROTO_PATH}/stepper_service.pb.c)
target_include_directories(stepper_service_google PUBLIC ${GENERATED_PROTO_PATH} $ENV{NANOPB_SRC_PATH})
add_dependencies(stepper_service_google generate_stepper_service)

add_executable(
	stepper_motor_app_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_protobuf_ipc_server.cpp
)

target_link_libraries(
	stepper_motor_app_tests
	PRIVATE
	stepper_protobuf_ipc_server

	gtest
)