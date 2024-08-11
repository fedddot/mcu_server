add_library(stepper_motor INTERFACE)
target_include_directories(stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor
    INTERFACE
    creator
    delay
    gpio
    gpo
)

add_subdirectory(tests EXCLUDE_FROM_ALL)