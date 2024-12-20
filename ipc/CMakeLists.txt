add_library(ipc_types INTERFACE)
target_include_directories(ipc_types INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(data_reader INTERFACE)
target_include_directories(data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(buffered_package_reader INTERFACE)
target_include_directories(buffered_package_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(buffered_package_reader INTERFACE data_reader ipc_types)

add_library(custom_data_reader INTERFACE)
target_include_directories(custom_data_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_data_reader INTERFACE data_reader)

add_library(data_writer INTERFACE)
target_include_directories(data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(package_writer INTERFACE)
target_include_directories(package_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(package_writer INTERFACE data_writer ipc_types)

add_library(custom_data_writer INTERFACE)
target_include_directories(custom_data_writer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_data_writer INTERFACE data_writer)

add_subdirectory(tests EXCLUDE_FROM_ALL)