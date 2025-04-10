#include <cstddef>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "sized_package_writer.hpp"
#include "sized_package_infra.hpp"

using namespace ipc;

TEST(ut_sized_package_writer, ctor_dtor_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = std::vector<char>(preamble_str.begin(), preamble_str.end());
	
	// WHEN
	SizedPackageWriter *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new SizedPackageWriter([](const std::vector<char>&) {}, preamble));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_sized_package_writer, read_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = std::vector<char>(preamble_str.begin(), preamble_str.end());
	const auto msg_str = std::string("test_msg");
	const auto msg = std::vector<char>(msg_str.begin(), msg_str.end());
	
	// WHEN
	auto instance = SizedPackageWriter(
		[preamble, msg](const std::vector<char>& raw_data)  {
			ASSERT_EQ(
				preamble.size() + sizeof(std::size_t) + msg.size(),
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
				SizedPackageInfra::encode_size(msg.size()),
				std::vector<char>(
					raw_data.begin() + preamble.size(),
					raw_data.begin() + preamble.size() + sizeof(std::size_t)
				)
			);
			ASSERT_EQ(
				msg,
				std::vector<char>(
					raw_data.begin() + preamble.size() + sizeof(std::size_t),
					raw_data.end()
				)
			);
		},
		preamble
	);

	// THEN
	ASSERT_NO_THROW(instance.write(msg));
}