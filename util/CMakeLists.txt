add_library(data_extractor INTERFACE)
target_include_directories(data_extractor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(custom_sender INTERFACE)
target_include_directories(custom_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_sender INTERFACE sender)

add_library(json_data_parser INTERFACE)
target_sources(json_data_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_data_parser.cpp)
target_include_directories(json_data_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_data_parser INTERFACE parser data_lib jsoncpp_lib)

add_library(json_data_serializer INTERFACE)
target_sources(json_data_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_data_serializer.cpp)
target_include_directories(json_data_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_data_serializer INTERFACE serializer data_lib jsoncpp_lib)

add_subdirectory(tests EXCLUDE_FROM_ALL)