#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "api_request_reader.hpp"
#include "package_utils.hpp"
#include "ring_queue.hpp"

using namespace ipc;

#define RING_BUFF_SIZE 	0xFFUL
#define HEADER_SIZE 	0x2UL

using ApiRequest = std::string;
using TestReader = ApiRequestReader<ApiRequest, HEADER_SIZE>;

TEST(ut_api_request_reader, ctor_dtor_sanity) {
	// GIVEN
	auto buff = RingDataBuffer<std::uint8_t, RING_BUFF_SIZE>();
	auto size_retriever = [](const IpcQueue<std::uint8_t>& queue) -> std::size_t {
		throw std::runtime_error("size retriever not implemented");
	};
	auto request_parser = [](const std::vector<std::uint8_t>& data) -> ApiRequest {
		throw std::runtime_error("request parser not implemented");
	};

	// WHEN
	TestReader *instance = nullptr;


	// THEN
	ASSERT_NO_THROW(
		instance = new TestReader(
			&buff,
			size_retriever,
			request_parser
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_api_request_reader, read_sanity) {
	// GIVEN
	auto size_retriever = [](const IpcQueue<std::uint8_t>& queue) -> std::size_t {
		const auto size_data = std::vector<std::uint8_t> {
			queue.inspect(0),
			queue.inspect(1),
		};
		return parse_package_size(size_data);
	};
	auto request_parser = [](const std::vector<std::uint8_t>& data) -> ApiRequest {
		throw std::runtime_error("request parser not implemented");
	};
	
	const auto api_request = std::string("test_msg");
	const auto api_request_size_encoded = serialize_package_size(api_request.size(), HEADER_SIZE);
	
	// WHEN
	auto buff = RingDataBuffer<std::uint8_t, RING_BUFF_SIZE>();
	auto instance = TestReader(
		&buff,
		size_retriever,
		request_parser
	);
	auto result = std::optional<ApiRequest>();

	// THEN
	// empty buffer
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);

	// add encoded message size
	for (const auto& byte: api_request_size_encoded) {
		buff.enqueue(byte);
	}
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);
	
	// add message
	for (const auto& byte: api_request) {
		buff.enqueue(static_cast<std::uint8_t>(byte));
	}
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_TRUE(result);
	ASSERT_EQ(api_request, result.value());
	ASSERT_EQ(0UL, buff.size());
}