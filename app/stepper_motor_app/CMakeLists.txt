set(STEPPER_SERVICE_PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/src)
set(STEPPER_SERVICE_PROTO_NAME stepper_service.proto)
set(GENERATED_PROTO_PATH ${CMAKE_BINARY_DIR}/proto_generated)
add_custom_command(
	OUTPUT ${GENERATED_PROTO_PATH}/stepper_service.pb.h ${GENERATED_PROTO_PATH}/stepper_service.pb.c
	COMMAND mkdir -p ${GENERATED_PROTO_PATH} && $ENV{NANOPB_SRC_PATH}/generator/protoc -I ${STEPPER_SERVICE_PROTO_PATH} --nanopb_out=${GENERATED_PROTO_PATH} ${STEPPER_SERVICE_PROTO_NAME}
	DEPENDS ${STEPPER_SERVICE_PROTO_PATH}/${STEPPER_SERVICE_PROTO_NAME}
)

add_custom_target(
	generate_stepper_service
	DEPENDS ${GENERATED_PROTO_PATH}/stepper_service.pb.h ${GENERATED_PROTO_PATH}/stepper_service.pb.c
)

add_library(stepper_service STATIC)
target_sources(stepper_service PRIVATE ${GENERATED_PROTO_PATH}/stepper_service.pb.c)
target_include_directories(stepper_service PUBLIC ${GENERATED_PROTO_PATH} $ENV{NANOPB_SRC_PATH})
add_dependencies(stepper_service generate_stepper_service)

add_library(stepper_protobuf_ipc_server INTERFACE)
target_include_directories(stepper_protobuf_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include $ENV{NANOPB_SRC_PATH})
target_link_libraries(
    stepper_protobuf_ipc_server
    INTERFACE
    ipc_server
    protobuf_ipc_server
    stepper_motor_request
    stepper_motor_response
    stepper_service
    pb_decode
    pb_encode
    pb_common
)

add_executable(test_stepper_motor_server ${CMAKE_CURRENT_LIST_DIR}/src/test_stepper_motor_server.cpp)
target_link_libraries(
    test_stepper_motor_server
    PRIVATE
    host
    http_ipc_server
    stepper_motor_request
    stepper_motor_response
    test_stepper_motor
    
    jsoncpp_static
    cpprest
)

add_subdirectory(tests EXCLUDE_FROM_ALL)