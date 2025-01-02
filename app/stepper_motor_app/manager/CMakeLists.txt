add_library(stepper_motor_data INTERFACE)
target_include_directories(stepper_motor_data INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor_data INTERFACE data_lib)

add_library(stepper_motor INTERFACE)
target_include_directories(stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor INTERFACE datastepper_motor_data_lib)

add_library(stepper_motor_manager INTERFACE)
target_include_directories(stepper_motor_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor_manager
    INTERFACE
    stepper_motor
    datastepper_motor_data_lib
)

add_subdirectory(tests EXCLUDE_FROM_ALL)