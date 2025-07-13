add_executable(
	thermostat_vendor_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_thermostat_vendor.cpp
)

target_link_libraries(
	thermostat_vendor_tests
	PRIVATE
    thermostat_manager
    thermostat_vendor
	
    gtest
)