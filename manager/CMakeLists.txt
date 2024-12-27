add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(tests EXCLUDE_FROM_ALL)