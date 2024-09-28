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