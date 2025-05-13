add_library(vendor_instance INTERFACE)
target_include_directories(vendor_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(vendor INTERFACE)
target_include_directories(vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(vendor_builder INTERFACE)
target_include_directories(vendor_builder INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(vendor_builder INTERFACE vendor manager vendor_instance)

add_subdirectory(tests EXCLUDE_FROM_ALL)