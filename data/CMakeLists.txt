add_library(data INTERFACE)
target_include_directories(data INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(object INTERFACE)
target_sources(object INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/object.cpp)
target_include_directories(object INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(object INTERFACE data)

add_library(array INTERFACE)
target_include_directories(array INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(array INTERFACE data)

add_library(string INTERFACE)
target_include_directories(string INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(string INTERFACE data)

add_library(integer INTERFACE)
target_include_directories(integer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(integer INTERFACE data)

add_library(data_lib INTERFACE)
target_link_libraries(data_lib INTERFACE data object array string integer)

add_subdirectory(tests EXCLUDE_FROM_ALL)