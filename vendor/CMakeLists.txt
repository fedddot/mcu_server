add_library(vendor_movable_instance INTERFACE)
target_include_directories(vendor_movable_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(vendor INTERFACE)
target_include_directories(vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(vendor INTERFACE manager vendor_movable_instance)

add_subdirectory(tests EXCLUDE_FROM_ALL)