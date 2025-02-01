add_library(ipc_server INTERFACE)
target_include_directories(ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(ipc_option INTERFACE)
target_include_directories(ipc_option INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(http_ipc_server INTERFACE)
target_include_directories(http_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    http_ipc_server
    INTERFACE
    ipc_server
    ipc_option

    cpprest
    boost_system
	crypto
    pthread
)

add_library(buffered_ipc_server INTERFACE)
target_include_directories(buffered_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    buffered_ipc_server
    INTERFACE
    ipc_option
    ipc_server
)

add_library(pb_common INTERFACE)
target_sources(pb_common INTERFACE $ENV{NANOPB_SRC_PATH}/pb_common.c)
target_include_directories(pb_common INTERFACE $ENV{NANOPB_SRC_PATH})

add_library(pb_encode INTERFACE)
target_sources(pb_encode INTERFACE $ENV{NANOPB_SRC_PATH}/pb_encode.c)
target_include_directories(pb_encode INTERFACE $ENV{NANOPB_SRC_PATH})

add_library(pb_decode INTERFACE)
target_sources(pb_decode INTERFACE $ENV{NANOPB_SRC_PATH}/pb_decode.c)
target_include_directories(pb_decode INTERFACE $ENV{NANOPB_SRC_PATH})

add_library(ring_buffer INTERFACE)
target_include_directories(ring_buffer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(protobuf_ipc_server INTERFACE)
target_include_directories(
    protobuf_ipc_server
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
    $ENV{NANOPB_SRC_PATH}
)
target_link_libraries(
    protobuf_ipc_server
    INTERFACE
    ipc_option
    ipc_server
    ring_buffer
)

add_subdirectory(tests EXCLUDE_FROM_ALL)