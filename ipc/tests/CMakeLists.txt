set(TEST_PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/src)
set(TEST_PROTO_NAME stepper_motor_service.proto)
set(GENERATED_PROTO_PATH ${CMAKE_BINARY_DIR}/proto_generated)
add_custom_command(
	OUTPUT ${GENERATED_PROTO_PATH}/stepper_motor_service.pb.h ${GENERATED_PROTO_PATH}/stepper_motor_service.pb.c
	COMMAND mkdir -p ${GENERATED_PROTO_PATH} && $ENV{NANOPB_SRC_PATH}/generator/protoc -I ${TEST_PROTO_PATH} --nanopb_out=${GENERATED_PROTO_PATH} ${TEST_PROTO_NAME}
	# COMMAND mkdir -p ${GENERATED_PROTO_PATH} && $ENV{NANOPB_SRC_PATH}/generator/nanopb_generator.py -I ${TEST_PROTO_PATH} -D ${GENERATED_PROTO_PATH} ${TEST_PROTO_NAME}
	DEPENDS ${TEST_PROTO_PATH}/${TEST_PROTO_NAME}
)

add_custom_target(
	generate_stepper_motor_service
	DEPENDS ${GENERATED_PROTO_PATH}/stepper_motor_service.pb.h ${GENERATED_PROTO_PATH}/stepper_motor_service.pb.c
)

add_library(stepper_motor_service INTERFACE)
target_sources(stepper_motor_service INTERFACE ${GENERATED_PROTO_PATH}/stepper_motor_service.pb.c)
target_include_directories(stepper_motor_service INTERFACE ${GENERATED_PROTO_PATH} $ENV{NANOPB_SRC_PATH})
add_dependencies(stepper_motor_service generate_stepper_motor_service)

add_executable(
	ipc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/src/ut_buffered_ipc_server.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/src/ut_http_ipc_server.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_protobuf_ipc_server.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_ring_buffer.cpp
)

target_link_libraries(
	ipc_tests
	PRIVATE
	http_ipc_server
	buffered_ipc_server
	ipc_option
	stepper_motor_service
	protobuf_ipc_server
	pb_common
	pb_encode
	pb_decode
	ring_buffer
	
	cpprest
	gtest
)

target_compile_definitions(
	ipc_tests
	PRIVATE
	# -DPB_BUFFER_ONLY=1
)