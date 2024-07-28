add_library(custom_task INTERFACE)
target_include_directories(custom_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_task INTERFACE task)

add_library(custom_creator INTERFACE)
target_include_directories(custom_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_creator INTERFACE creator)

add_library(custom_parser INTERFACE)
target_include_directories(custom_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_parser INTERFACE parser)

add_library(json_data_parser INTERFACE)
target_sources(json_data_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_data_parser.cpp)
target_include_directories(json_data_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_data_parser INTERFACE parser data_lib jsoncpp_static)

add_library(json_data_serializer INTERFACE)
target_sources(json_data_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_data_serializer.cpp)
target_include_directories(json_data_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(json_data_serializer INTERFACE serializer data_lib jsoncpp_static)

add_subdirectory(tests EXCLUDE_FROM_ALL)