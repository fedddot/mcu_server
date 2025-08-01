#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "api_response_writer.hpp"
#include "package_utils.hpp"

using namespace ipc;

#define HEADER_SIZE 	0x2UL

using ApiResponse = std::string;
using TestWriter = ApiResponseWriter<ApiResponse, HEADER_SIZE>;

TEST(ut_api_response_writer, ctor_dtor_sanity) {
	// GIVEN
	auto header_generator = [](const std::vector<std::uint8_t>& package_data, const std::size_t& header_size) -> std::vector<std::uint8_t> {
		throw std::runtime_error("not implemented");
	};
	auto raw_data_writer = [](const std::vector<std::uint8_t>& data) {
		throw std::runtime_error("not implemented");
	};
	auto response_serializer = [](const ApiResponse& response) -> std::vector<std::uint8_t> {
		throw std::runtime_error("not implemented");
	};

	// WHEN
	TestWriter *instance = nullptr;


	// THEN
	ASSERT_NO_THROW(
		instance = new TestWriter(
			header_generator,
			raw_data_writer,
			response_serializer
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

// TEST(ut_api_response_writer, read_sanity) {
// 	// GIVEN
// 	auto size_retriever = [](const IpcQueue<std::uint8_t>& queue) -> std::size_t {
// 		const auto size_data = std::vector<std::uint8_t> {
// 			queue.inspect(0),
// 			queue.inspect(1),
// 		};
// 		return parse_package_size(size_data);
// 	};
// 	auto request_parser = [](const std::vector<std::uint8_t>& data) -> ApiResponse {
// 		throw std::runtime_error("request parser not implemented");
// 	};
	
// 	const auto api_request = std::string("test_msg");
// 	const auto api_request_size_encoded = serialize_package_size(api_request.size(), HEADER_SIZE);
	
// 	// WHEN
// 	auto buff = RingDataBuffer<std::uint8_t, RING_BUFF_SIZE>();
// 	auto instance = TestWriter(
// 		&buff,
// 		size_retriever,
// 		request_parser
// 	);
// 	auto result = std::optional<ApiResponse>();

// 	// THEN
// 	// empty buffer
// 	ASSERT_NO_THROW(result = instance.read());
// 	ASSERT_FALSE(result);

// 	// add encoded message size
// 	for (const auto& byte: api_request_size_encoded) {
// 		buff.enqueue(byte);
// 	}
// 	ASSERT_NO_THROW(result = instance.read());
// 	ASSERT_FALSE(result);
	
// 	// add message
// 	for (const auto& byte: api_request) {
// 		buff.enqueue(static_cast<std::uint8_t>(byte));
// 	}
// 	ASSERT_NO_THROW(result = instance.read());
// 	ASSERT_TRUE(result);
// 	ASSERT_EQ(api_request, result.value());
// 	ASSERT_EQ(0UL, buff.size());
// }