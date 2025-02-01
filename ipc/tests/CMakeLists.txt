set(TEST_PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/src)
set(TEST_PROTO_NAME test_service.proto)
set(GENERATED_PROTO_PATH ${CMAKE_BINARY_DIR}/proto_generated)
add_custom_command(
	OUTPUT ${GENERATED_PROTO_PATH}/test_service.pb.h ${GENERATED_PROTO_PATH}/test_service.pb.c
	COMMAND mkdir -p ${GENERATED_PROTO_PATH} && $ENV{NANOPB_SRC_PATH}/generator/protoc -I ${TEST_PROTO_PATH} --nanopb_out=${GENERATED_PROTO_PATH} ${TEST_PROTO_NAME}
	DEPENDS ${TEST_PROTO_PATH}/${TEST_PROTO_NAME}
)

add_custom_target(
	generate_test_service
	DEPENDS ${GENERATED_PROTO_PATH}/test_service.pb.h ${GENERATED_PROTO_PATH}/test_service.pb.c
)

add_library(test_service INTERFACE)
target_sources(test_service INTERFACE ${GENERATED_PROTO_PATH}/test_service.pb.c)
target_include_directories(test_service INTERFACE ${GENERATED_PROTO_PATH} $ENV{NANOPB_SRC_PATH})
add_dependencies(test_service generate_test_service)

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
	test_service
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
)