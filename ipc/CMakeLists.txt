add_library(ipc_types INTERFACE)
target_include_directories(ipc_types INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(data_reader INTERFACE)
target_include_directories(data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(buffered_package_reader INTERFACE)
target_include_directories(buffered_package_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(buffered_package_reader INTERFACE data_reader)

add_library(data_writer INTERFACE)
target_include_directories(data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(tests EXCLUDE_FROM_ALL)