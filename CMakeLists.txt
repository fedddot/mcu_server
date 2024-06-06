add_library(gpio INTERFACE)
target_include_directories(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(gpi INTERFACE)
target_include_directories(gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpi INTERFACE gpio)

add_library(gpo INTERFACE)
target_include_directories(gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpo INTERFACE gpio)

add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(parser INTERFACE)
target_include_directories(parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(serializer INTERFACE)
target_include_directories(serializer INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(server INTERFACE)
target_include_directories(server INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    server
    INTERFACE
    engine
    
    creator
    server_factory
    server_creator
    server_task

    inventory
    server_inventory
    
    data_lib

    gpio
    gpi
    gpo
)

add_subdirectory(engine)
add_subdirectory(utils)
add_subdirectory(parse)
add_subdirectory(data)
add_subdirectory(tests EXCLUDE_FROM_ALL)