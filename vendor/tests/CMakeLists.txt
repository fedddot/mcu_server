add_executable(
	vendor_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_vendor.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_vendor_builder.cpp
)

target_link_libraries(
	vendor_tests
	PRIVATE
    vendor
	vendor_builder
    gtest
)