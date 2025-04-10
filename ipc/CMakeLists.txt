add_library(request_reader INTERFACE)
target_include_directories(request_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(response_writer INTERFACE)
target_include_directories(response_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(protobuf)
add_subdirectory(sized_package)
add_subdirectory(tests EXCLUDE_FROM_ALL)