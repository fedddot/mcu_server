add_library(manager_instance INTERFACE)
target_include_directories(manager_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager_instance_reference INTERFACE)
target_include_directories(manager_instance_reference INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(movement_manager)
add_subdirectory(thermostat_manager)