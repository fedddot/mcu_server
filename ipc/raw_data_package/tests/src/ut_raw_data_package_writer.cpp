#include <cstddef>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "raw_data_package_writer.hpp"
#include "raw_data_package_descriptor.hpp"

using namespace ipc;

TEST(ut_raw_data_package_writer, ctor_dtor_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = std::vector<char>(preamble_str.begin(), preamble_str.end());
	const auto size_field_len = std::size_t(4UL);
	
	// WHEN
	RawDataPackageWriter *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new RawDataPackageWriter([](const std::vector<char>&) {}, preamble, size_field_len));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_raw_data_package_writer, read_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = std::vector<char>(preamble_str.begin(), preamble_str.end());
	const auto msg_str = std::string("test_msg");
	const auto msg = std::vector<char>(msg_str.begin(), msg_str.end());
	const auto size_field_len = std::size_t(4UL);
	
	// WHEN
	auto instance = RawDataPackageWriter(
		[preamble, msg, size_field_len](const std::vector<char>& raw_data)  {
			ASSERT_EQ(
				preamble.size() + size_field_len + msg.size(),
				raw_data.size()
			);
			ASSERT_EQ(
				preamble,
				std::vector<char>(
					raw_data.begin(),
					raw_data.begin() + preamble.size()
				)
			);
			ASSERT_EQ(
				DefaultPackageSizeSerializer(size_field_len).transform(msg.size()),
				std::vector<char>(
					raw_data.begin() + preamble.size(),
					raw_data.begin() + preamble.size() + size_field_len
				)
			);
			ASSERT_EQ(
				msg,
				std::vector<char>(
					raw_data.begin() + preamble.size() + size_field_len,
					raw_data.end()
				)
			);
		},
		preamble,
		size_field_len
	);

	// THEN
	ASSERT_NO_THROW(instance.write(msg));
}