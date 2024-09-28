add_library(test_resource INTERFACE)
target_include_directories(test_resource INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(test_resource INTERFACE resource)

add_executable(
	vendor_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_vendor.cpp
)

target_link_libraries(
	vendor_tests
	PRIVATE
    vendor
	test_resource

	gtest
)