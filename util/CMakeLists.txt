add_library(custom_task INTERFACE)
target_include_directories(custom_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_task INTERFACE task)

add_library(custom_creator INTERFACE)
target_include_directories(custom_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_creator INTERFACE creator)