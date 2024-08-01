add_library(test_delay INTERFACE)
target_include_directories(test_delay INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_delay INTERFACE delay)

add_library(test_gpi INTERFACE)
target_include_directories(test_gpi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpi INTERFACE gpi)

add_library(test_gpo INTERFACE)
target_include_directories(test_gpo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_gpo INTERFACE gpo)

add_library(test_platform INTERFACE)
target_include_directories(test_platform INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    test_platform
    INTERFACE
    test_delay
    gpio
    platform
    test_gpi
    test_gpo
)

add_executable(
	test_platform_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_test_platform.cpp
)

target_link_libraries(
	test_platform_tests
	PRIVATE
    test_platform

	gtest
)