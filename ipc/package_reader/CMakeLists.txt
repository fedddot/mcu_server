add_library(package_header INTERFACE)
target_include_directories(package_header INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(package_utils INTERFACE)
target_sources(package_utils INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/package_utils.cpp)
target_include_directories(package_utils INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(package_reader INTERFACE)
target_include_directories(package_reader INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    package_reader
    INTERFACE
    ipc_data_reader
    package_header
)

add_subdirectory(tests EXCLUDE_FROM_ALL)