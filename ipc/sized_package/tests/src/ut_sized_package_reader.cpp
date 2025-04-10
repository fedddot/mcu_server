#include <optional>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "sized_package_reader.hpp"
#include "sized_package_infra.hpp"

using namespace ipc;

TEST(ut_sized_package_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = std::vector<char>(preamble_str.begin(), preamble_str.end());
	
	// WHEN
	auto buff = std::vector<char>();
	SizedPackageReader *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new SizedPackageReader(&buff, preamble));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_sized_package_reader, read_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = std::vector<char>(preamble_str.begin(), preamble_str.end());
	const auto msg_str = std::string("test_msg");
	const auto msg = std::vector<char>(msg_str.begin(), msg_str.end());
	const auto msg_size_encoded = SizedPackageInfra::encode_size(msg.size());
	const auto junk_before_str = std::string("junk");
	const auto junk_before = std::vector<char>(junk_before_str.begin(), junk_before_str.end());
	
	// WHEN
	auto buff = std::vector<char>();

	auto instance = SizedPackageReader(&buff, preamble);
	auto result = std::optional<std::vector<char>>();

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
	ASSERT_EQ(msg, *result);

	ASSERT_TRUE(buff.empty());
}