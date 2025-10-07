add_library(service INTERFACE)
target_include_directories(service INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(gateway_service)
add_subdirectory(motor_drive_service)