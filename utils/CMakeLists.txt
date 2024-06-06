add_library(server_creator INTERFACE)
target_include_directories(server_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_creator INTERFACE creator)

add_library(server_parser INTERFACE)
target_include_directories(server_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_parser INTERFACE parser)

add_library(server_serializer INTERFACE)
target_include_directories(server_serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_serializer INTERFACE serializer)

add_library(server_task INTERFACE)
target_include_directories(server_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_task INTERFACE task)