add_library(manager_instance INTERFACE)
target_include_directories(manager_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(movement_manager)