#include <cstdint>
#include <optional>

#include "gtest/gtest.h"

#include "api_request_reader.hpp"
#include "ring_queue.hpp"

using namespace host_tools;
using ApiRequest = std::string;

TEST(ut_api_request_reader, sanity) {
	// GIVEN
	const auto msg_size = 3UL;
	const auto encoded_package_size_length = 2UL;
	const auto buff_size = encoded_package_size_length + msg_size;
	const auto test_request = ApiRequest("abc");
	using TestBuffer = RingDataBuffer<std::uint8_t, buff_size>;
	
	// WHEN
	TestBuffer buffer;
	ApiRequestReader<ApiRequest, encoded_package_size_length> instance(
		&buffer,
		[](const std::vector<std::uint8_t>& data) -> ApiRequest {
			return ApiRequest(data.begin(), data.end());
		}
	);

	// THEN
	ASSERT_EQ(std::nullopt, instance());

	// WHEN
	buffer.push_back(static_cast<std::uint8_t>(msg_size & 0xFF));
	buffer.push_back(static_cast<std::uint8_t>((msg_size >> CHAR_BIT) & 0xFF));
	// THEN
	ASSERT_EQ(std::nullopt, instance());
	
	// WHEN
	for (const auto ch : test_request) {
		buffer.push_back(static_cast<std::uint8_t>(ch));
	}
	const auto result = instance();
	// THEN
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(result.value(), test_request);
	ASSERT_EQ(buffer.size(), 0UL);
}