add_executable(
	movement_vendor_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_movement_vendor.cpp
)

target_link_libraries(
	movement_vendor_tests
	PRIVATE
    movement_vendor
	custom_manager
    gtest
)