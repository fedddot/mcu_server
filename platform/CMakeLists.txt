add_library(gpio INTERFACE)
target_include_directories(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(gpi INTERFACE)
target_include_directories(gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpi INTERFACE gpio)

add_library(gpo INTERFACE)
target_include_directories(gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpo INTERFACE gpio)

add_library(message_receiver INTERFACE)
target_include_directories(message_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(message_sender INTERFACE)
target_include_directories(message_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(platform INTERFACE)
target_include_directories(platform INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    platform
    INTERFACE 
    gpio
    inventory
    message_receiver
    message_sender
)

add_subdirectory(util)
add_subdirectory(tests EXCLUDE_FROM_ALL)