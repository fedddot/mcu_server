add_library(manager_instance INTERFACE)
target_include_directories(manager_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(custom_manager INTERFACE)
target_include_directories(custom_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_manager INTERFACE manager)

add_subdirectory(stepper_motor_manager)
add_subdirectory(movement_manager)