add_executable(
	apps_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_gpio_host.cpp
)

target_link_libraries(
	apps_tests
	PRIVATE
    gpio_host	
	gtest
)