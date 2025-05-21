#include <cstddef>
#include <string>

#include "gtest/gtest.h"

#include "ipc_data.hpp"
#include "raw_data_package_writer.hpp"
#include "raw_data_package_descriptor.hpp"
#include "raw_data_package_utils.hpp"

using namespace ipc;

TEST(ut_raw_data_package_writer, ctor_dtor_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = RawData(preamble_str.begin(), preamble_str.end());
	const auto size_field_len = std::size_t(4UL);
	const auto descriptor = RawDataPackageDescriptor(preamble, size_field_len);
	const auto raw_data_writer = [](const RawData&) {};
	
	// WHEN
	RawDataPackageWriter *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		instance = new RawDataPackageWriter(
			descriptor,
			serialize_package_size,
			raw_data_writer
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_raw_data_package_writer, read_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = RawData(preamble_str.begin(), preamble_str.end());
	const auto size_field_len = std::size_t(4UL);
	const auto descriptor = RawDataPackageDescriptor(preamble, size_field_len);

	const auto msg_str = std::string("test_msg");
	const auto msg = RawData(msg_str.begin(), msg_str.end());
	const auto raw_data_writer = [](const RawData&) {};
	
	// WHEN
	auto instance = RawDataPackageWriter(
		descriptor,
		serialize_package_size,
		raw_data_writer
	);

	// THEN
	ASSERT_NO_THROW(instance.write(msg));
}