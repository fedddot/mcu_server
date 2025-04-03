add_library(pb_common INTERFACE)
target_sources(pb_common INTERFACE $ENV{NANOPB_SRC_PATH}/pb_common.c)
target_include_directories(pb_common INTERFACE $ENV{NANOPB_SRC_PATH})

add_library(pb_decode INTERFACE)
target_sources(pb_decode INTERFACE $ENV{NANOPB_SRC_PATH}/pb_decode.c)
target_include_directories(pb_decode INTERFACE $ENV{NANOPB_SRC_PATH})


add_library(pb_request_reader INTERFACE)
target_include_directories(pb_request_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(pb_request_reader INTERFACE request_reader pb_common pb_decode)

add_library(pb_response_writer INTERFACE)
target_include_directories(pb_response_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(pb_response_writer INTERFACE response_writer)

add_subdirectory(tests EXCLUDE_FROM_ALL)