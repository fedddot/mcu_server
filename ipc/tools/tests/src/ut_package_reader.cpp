#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

#include "gtest/gtest.h"
#include <vector>

#include "ipc_data_reader.hpp"
#include "ipc_queue.hpp"
#include "package_reader.hpp"
#include "package_utils.hpp"
#include "ring_queue.hpp"

using namespace ipc;

class Header {
public:
	Header(const std::string& preamble, const std::size_t& data_size, const std::string& expected_preamble): m_preamble(preamble), m_data_size(data_size), m_expected_preamble(expected_preamble) {}
	Header(const Header&) = default;
	Header& operator=(const Header&) = default;
	virtual ~Header() noexcept = default;
	std::size_t data_size() const {
		return m_data_size;
	}
	bool validate() const {
		return m_expected_preamble == m_preamble;
	}
private:
	std::string m_preamble;
	std::size_t m_data_size;
	std::string m_expected_preamble;
};

#define RING_BUFF_SIZE 	0xFFUL
#define HEADER_SIZE 	0x4UL
#define PREAMBLE_SIZE   0x3UL
#define PREAMBLE		"abc"

class HeaderReader: public DataReader<std::optional<Header>(IpcQueue<std::uint8_t> *)> {
public:
	HeaderReader() = default;
	HeaderReader(const HeaderReader&) = default;
	HeaderReader(HeaderReader&&) = default;
	HeaderReader& operator=(const HeaderReader&) = default;
	virtual ~HeaderReader() noexcept = default;
	std::optional<Header> read(IpcQueue<std::uint8_t> *buff_ptr) const override {
		if (buff_ptr->size() < HEADER_SIZE) {
			return std::nullopt;
		}
		std::string preamble(PREAMBLE_SIZE, '\0');
		for (auto i = 0; i < PREAMBLE_SIZE; ++i) {
			preamble[i] = buff_ptr->dequeue();
		}
		std::vector<std::uint8_t> size_data(HEADER_SIZE - PREAMBLE_SIZE, '\0');
		for (auto i = 0; i < HEADER_SIZE - PREAMBLE_SIZE; ++i) {
			size_data[i] = buff_ptr->dequeue();
		}
		return Header(preamble, parse_package_size(size_data), PREAMBLE);
	}
};

TEST(ut_package_reader, ctor_dtor_sanity) {
	// GIVEN
	auto buff = RingQueue<std::uint8_t, RING_BUFF_SIZE>();
	
	// WHEN
	PackageReader<Header, HeaderReader> *instance = nullptr;


	// THEN
	ASSERT_NO_THROW(
		(
			instance = new PackageReader<Header, HeaderReader>(
				&buff,
				HeaderReader()
			)
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
	const auto msg_size_encoded = serialize_package_size(msg_str.size(), HEADER_SIZE - PREAMBLE_SIZE);
	
	// WHEN
	auto buff = RingQueue<std::uint8_t, RING_BUFF_SIZE>();
	auto instance = PackageReader<Header, HeaderReader>(
		&buff,
		HeaderReader()
	);
	auto result = std::optional<std::vector<std::uint8_t>>();

	// THEN
	// empty buffer
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_FALSE(result);

	// add preamble
	for (auto i = 0; i < PREAMBLE_SIZE; i++) {
		buff.enqueue(PREAMBLE[i]);
	}
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