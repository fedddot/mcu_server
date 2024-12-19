add_library(json_data_parser INTERFACE)
target_sources(json_data_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_data_parser.cpp)
target_include_directories(json_data_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_data_parser INTERFACE data_lib jsoncpp_static)

add_library(json_request_parser INTERFACE)
target_include_directories(json_request_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_request_parser INTERFACE data_lib json_data_parser server_types request)

add_library(json_data_serializer INTERFACE)
target_sources(json_data_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_data_serializer.cpp)
target_include_directories(json_data_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_data_serializer INTERFACE data_lib jsoncpp_static)

add_library(json_response_serializer INTERFACE)
target_include_directories(json_response_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_response_serializer INTERFACE data_lib json_data_serializer server_types response)

add_library(buffered_ipc_connection INTERFACE)
target_include_directories(buffered_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(buffered_ipc_connection INTERFACE ipc_connection request response)

add_library(custom_server INTERFACE)
target_include_directories(custom_server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    custom_server
    INTERFACE
    data_lib
    ipc_connection
    request
    response
    server
    server_exception
    server_types
    vendor
)

add_library(http_ipc_connection INTERFACE)
target_include_directories(http_ipc_connection INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    http_ipc_connection
    INTERFACE
    data_lib
    ipc_connection
    json_data_parser
    json_data_serializer
    request
    response
    server_types
    -lcpprest
    -lpthread
    -lssl
    -lcrypto
)

add_subdirectory(tests EXCLUDE_FROM_ALL)