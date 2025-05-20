#include <cstddef>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "movement_host_builder.hpp"
#include "raw_data_package_descriptor.hpp"

using namespace ipc;
// using namespace manager;
using namespace host;

using AxesConfig = std::string;

TEST(ut_movement_host_builder, ctor_dtor_sanity) {
	// WHEN
	MovementHostBuilder<AxesConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new MovementHostBuilder<AxesConfig>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_host_builder, build_sanity) {
	using RawData = MovementHostBuilder<AxesConfig>::RawData;
	
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = RawData(preamble_str.begin(), preamble_str.end());
	const auto encoded_size_length = 4UL;
	
	// WHEN
	RawData buffer;
	MovementHostBuilder<AxesConfig> instance;
	instance
		.set_raw_data_buffer_ptr(&buffer)
		.set_package_descriptor(RawDataPackageDescriptor(preamble, encoded_size_length))
		.set_package_size_parser(
			[](const RawDataPackageDescriptor&, const RawData&) -> std::size_t {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		);

	// THEN
	ASSERT_NO_THROW(
		{
			const auto host = instance.build();
		}
	);
}