#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "package_reader.hpp"
#include "package_utils.hpp"
#include "ring_queue.hpp"

using namespace ipc;

#define RING_BUFF_SIZE 	0xFFUL
#define HEADER_SIZE 	0x2UL

TEST(ut_package_reader, ctor_dtor_sanity) {
	// GIVEN
	auto buff = RingDataBuffer<std::uint8_t, RING_BUFF_SIZE>();
	auto size_retriever = [](const IpcQueue<std::uint8_t>& queue) -> std::size_t {
		throw std::runtime_error("size retriever not implemented");
	};

	// WHEN
	PackageReader<HEADER_SIZE> *instance = nullptr;


	// THEN
	ASSERT_NO_THROW(
		instance = new PackageReader<HEADER_SIZE>(
			&buff,
			size_retriever
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_package_reader, read_sanity) {
	// GIVEN
	auto size_retriever = [](const IpcQueue<std::uint8_t>& queue) -> std::size_t {
		const auto size_data = std::vector<std::uint8_t> {
			queue.inspect(0),
			queue.inspect(1),
		};
		return parse_package_size(size_data);
	};
	
	const auto msg_str = std::string("test_msg");
	const auto msg_size_encoded = serialize_package_size(msg_str.size(), HEADER_SIZE);
	
	// WHEN
	auto buff = RingDataBuffer<std::uint8_t, RING_BUFF_SIZE>();
	auto instance = PackageReader<HEADER_SIZE>(
		&buff,
		size_retriever
	);
	auto result = std::optional<std::vector<std::uint8_t>>();

	// THEN
	// empty buffer
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);

	// add encoded message size
	for (const auto& byte: msg_size_encoded) {
		buff.enqueue(byte);
	}
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);
	
	// add message
	for (const auto& byte: msg_str) {
		buff.enqueue(static_cast<std::uint8_t>(byte));
	}
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_TRUE(result);
	ASSERT_EQ(msg_str, std::string(result->begin(), result->end()));
	ASSERT_EQ(0UL, buff.size());
}