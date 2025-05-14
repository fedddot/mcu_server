add_library(vendor_api_request INTERFACE)
target_include_directories(vendor_api_request INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(vendor_api_response INTERFACE)
target_include_directories(vendor_api_response INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(vendor_instance INTERFACE)
target_include_directories(vendor_instance INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(vendor INTERFACE)
target_include_directories(vendor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(vendor INTERFACE vendor_api_request vendor_api_response)

add_library(vendor_builder INTERFACE)
target_include_directories(vendor_builder INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(vendor_builder INTERFACE vendor manager vendor_instance)

add_subdirectory(movement_vendor EXCLUDE_FROM_ALL)
add_subdirectory(tests EXCLUDE_FROM_ALL)