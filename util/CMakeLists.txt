add_library(custom_sender INTERFACE)
target_include_directories(custom_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_sender INTERFACE sender)

add_library(custom_listener INTERFACE)
target_include_directories(custom_listener INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_listener INTERFACE listener)

add_library(custom_receiver INTERFACE)
target_include_directories(custom_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_receiver INTERFACE data_receiver listener)

add_library(json_data_parser INTERFACE)
target_sources(json_data_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_data_parser.cpp)
target_include_directories(json_data_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_data_parser INTERFACE parser data_lib jsoncpp_lib)

add_library(json_data_serializer INTERFACE)
target_sources(json_data_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_data_serializer.cpp)
target_include_directories(json_data_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_data_serializer INTERFACE serializer data_lib jsoncpp_lib)

add_subdirectory(tests EXCLUDE_FROM_ALL)