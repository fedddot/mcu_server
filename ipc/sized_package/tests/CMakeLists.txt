add_executable(
	sized_package_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_sized_package_reader.cpp
)

target_link_libraries(
	sized_package_tests
	PRIVATE
    sized_package_reader
	sized_package_infra
	gtest
)