add_library(service_instance INTERFACE)
target_include_directories(service_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(service INTERFACE)
target_include_directories(service INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(movement_service EXCLUDE_FROM_ALL)
add_subdirectory(thermostat_service EXCLUDE_FROM_ALL)
add_subdirectory(tests EXCLUDE_FROM_ALL)