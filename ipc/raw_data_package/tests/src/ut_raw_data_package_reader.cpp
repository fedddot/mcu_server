#include <cstddef>
#include <optional>
#include <string>

#include "gtest/gtest.h"

#include "ipc_data.hpp"
#include "raw_data_package_descriptor.hpp"
#include "raw_data_package_reader.hpp"
#include "raw_data_package_utils.hpp"

using namespace ipc;

TEST(ut_raw_data_package_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = RawData(preamble_str.begin(), preamble_str.end());
	const auto size_field_len = std::size_t(4UL);
	const auto descriptor = RawDataPackageDescriptor(preamble, size_field_len);
	
	// WHEN
	auto buff = RawData();
	RawDataPackageReader *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		instance = new RawDataPackageReader(
			&buff,
			descriptor,
			parse_package_size
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_raw_data_package_reader, read_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = RawData(preamble_str.begin(), preamble_str.end());
	const auto size_field_len = std::size_t(4UL);
	const auto descriptor = RawDataPackageDescriptor(preamble, size_field_len);

	const auto msg_str = std::string("test_msg");
	const auto msg = RawData(msg_str.begin(), msg_str.end());
	const auto msg_size_encoded = serialize_package_size(descriptor, msg.size());
	const auto junk_before_str = std::string("junk");
	const auto junk_before = RawData(junk_before_str.begin(), junk_before_str.end());
	
	// WHEN
	auto buff = RawData();

	auto instance = RawDataPackageReader(
		&buff,
		descriptor,
		parse_package_size
	);
	auto result = std::optional<Instance<RawData>>();

	// THEN
	// empty buffer
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);
	
	// junk data should be ignored
	buff.insert(
		buff.end(),
		junk_before.begin(),
		junk_before.end()
	);
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);

	buff.insert(
		buff.end(),
		preamble.begin(),
		preamble.end()
	);
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);
	
	buff.insert(
		buff.end(),
		msg_size_encoded.begin(),
		msg_size_encoded.end()
	);
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);
	
	buff.insert(
		buff.end(),
		msg.begin(),
		msg.end()
	);
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_TRUE(result);
	ASSERT_EQ(msg, result->get());

	ASSERT_TRUE(buff.empty());
}