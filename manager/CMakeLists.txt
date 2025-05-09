add_library(provider INTERFACE)
target_include_directories(provider INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager_config INTERFACE)
target_include_directories(manager_config INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager_clonable INTERFACE)
target_include_directories(manager_clonable INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(custom_manager INTERFACE)
target_include_directories(custom_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_manager INTERFACE manager manager_clonable)

add_subdirectory(stepper_motor_manager)
add_subdirectory(movement_manager)