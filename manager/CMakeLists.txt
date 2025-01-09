add_library(provider INTERFACE)
target_include_directories(provider INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(manager INTERFACE)
target_include_directories(manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)