add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(platform INTERFACE)
target_include_directories(platform INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(platform INTERFACE gpio)

add_subdirectory(stepper_motor)
add_subdirectory(tests EXCLUDE_FROM_ALL)