add_library(provider INTERFACE)
target_include_directories(provider INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(providers INTERFACE)
target_include_directories(providers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(providers INTERFACE provider)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(manager INTERFACE providers)

# add_subdirectory(gpio_manager)