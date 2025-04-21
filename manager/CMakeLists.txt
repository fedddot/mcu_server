add_library(provider INTERFACE)
target_include_directories(provider INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager_config INTERFACE)
target_include_directories(manager_config INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(clonable_manager INTERFACE)
target_include_directories(clonable_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(clonable_manager INTERFACE manager)

add_library(custom_manager INTERFACE)
target_include_directories(custom_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_manager INTERFACE manager clonable_manager)

add_subdirectory(stepper_motor_manager)
add_subdirectory(tests EXCLUDE_FROM_ALL)