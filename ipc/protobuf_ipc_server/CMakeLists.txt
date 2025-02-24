add_library(pb_include INTERFACE)
target_include_directories(pb_include INTERFACE $ENV{NANOPB_SRC_PATH})

add_library(pb_common INTERFACE)
target_sources(pb_common INTERFACE $ENV{NANOPB_SRC_PATH}/pb_common.c)
target_link_libraries(pb_common INTERFACE pb_include)

add_library(pb_encode INTERFACE)
target_sources(pb_encode INTERFACE $ENV{NANOPB_SRC_PATH}/pb_encode.c)
target_link_libraries(pb_encode INTERFACE pb_include)

add_library(pb_decode INTERFACE)
target_sources(pb_decode INTERFACE $ENV{NANOPB_SRC_PATH}/pb_decode.c)
target_link_libraries(pb_decode INTERFACE pb_include)

add_library(protobuf_ipc_server INTERFACE)
target_include_directories(protobuf_ipc_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(protobuf_ipc_server INTERFACE ipc_server custom_ipc_server pb_include)

add_subdirectory(tests EXCLUDE_FROM_ALL)