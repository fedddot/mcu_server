add_library(raw_data_package_descriptor INTERFACE)
target_include_directories(raw_data_package_descriptor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(raw_data_package_utils INTERFACE)
target_sources(raw_data_package_utils INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/raw_data_package_utils.cpp)
target_include_directories(raw_data_package_utils INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(raw_data_package_utils INTERFACE raw_data_package_descriptor)

add_library(raw_data_package_reader INTERFACE)
target_include_directories(raw_data_package_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    raw_data_package_reader
    INTERFACE
    ipc_data_reader
    clonable_ipc_data_reader
    raw_data_package_descriptor
)

add_library(raw_data_package_writer INTERFACE)
target_include_directories(raw_data_package_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    raw_data_package_writer
    INTERFACE
    ipc_data_writer
    clonable_ipc_data_writer
    raw_data_package_descriptor
)

add_subdirectory(tests EXCLUDE_FROM_ALL)