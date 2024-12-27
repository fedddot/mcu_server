add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager_factory INTERFACE)
target_include_directories(manager_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(manager_factory INTERFACE manager)

add_subdirectory(tests EXCLUDE_FROM_ALL)