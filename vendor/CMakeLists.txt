add_library(vendor_instance INTERFACE)
target_include_directories(vendor_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(vendor INTERFACE)
target_include_directories(vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(movement_vendor EXCLUDE_FROM_ALL)
add_subdirectory(tests EXCLUDE_FROM_ALL)