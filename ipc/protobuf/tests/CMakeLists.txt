set(EXAMPLE_SERVICE_PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/src)
set(EXAMPLE_SERVICE_PROTO_NAME example.proto)
set(GENERATED_PROTO_PATH ${CMAKE_BINARY_DIR}/proto_generated)
add_custom_command(
	OUTPUT ${GENERATED_PROTO_PATH}/example.pb.h ${GENERATED_PROTO_PATH}/example.pb.c
	COMMAND mkdir -p ${GENERATED_PROTO_PATH} && $ENV{NANOPB_SRC_PATH}/generator/protoc -I ${EXAMPLE_SERVICE_PROTO_PATH} --nanopb_out=${GENERATED_PROTO_PATH} ${EXAMPLE_SERVICE_PROTO_NAME}
	DEPENDS ${EXAMPLE_SERVICE_PROTO_PATH}/${EXAMPLE_SERVICE_PROTO_NAME}
)

add_custom_target(
	generate_example
	DEPENDS ${GENERATED_PROTO_PATH}/example.pb.h ${GENERATED_PROTO_PATH}/example.pb.c
)

add_library(example STATIC)
target_sources(example PRIVATE ${GENERATED_PROTO_PATH}/example.pb.c)
target_include_directories(example PUBLIC ${GENERATED_PROTO_PATH} $ENV{NANOPB_SRC_PATH})
add_dependencies(example generate_example)

add_executable(
	protobuf_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_pb_request_reader.cpp
)

target_link_libraries(
	protobuf_tests
	PRIVATE
    pb_request_reader
    pb_response_writer
	example
	gtest
)