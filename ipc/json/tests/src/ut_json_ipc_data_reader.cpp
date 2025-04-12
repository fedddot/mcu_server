#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"
// #include "json/value.h"

#include "json_ipc_data_reader.hpp"
#include "ipc_data_reader.hpp"

using namespace ipc;

using TestRequest = std::string;

class TestRawReader: public IpcDataReader<std::vector<char>> {
public:
	TestRawReader() = default;
	TestRawReader(const TestRawReader&) = default;
	TestRawReader& operator=(const TestRawReader&) = default;
	
	std::optional<std::vector<char>> read() override {
		return m_data_option;
	}
	void update_data(const std::vector<char>& data) {
		m_data_option = data;
	}
private:
	std::optional<std::vector<char>> m_data_option;
};

TEST(ut_json_ipc_data_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto raw_data_reader = SharedIpcDataReader<std::vector<char>>(new TestRawReader());
	
	// WHEN
	JsonIpcDataReader<TestRequest> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		instance = new JsonIpcDataReader<TestRequest>(
			raw_data_reader,
			[](const std::vector<char>&) -> TestRequest {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

static std::vector<char> serialize_request(const TestRequest& request);

TEST(ut_json_ipc_data_reader, read_sanity) {
	// GIVEN
	const auto request = TestRequest("request");
	const auto request_json_raw = serialize_request(request);
	
	// WHEN
	auto raw_reader = TestRawReader();
	raw_reader.update_data(request_json_raw);
	auto instance = JsonIpcDataReader<TestRequest>(
		SharedIpcDataReader<std::vector<char>>(new TestRawReader(raw_reader)),
		[](const std::vector<char>&) -> TestRequest {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	auto result = std::optional<TestRequest>();

	// THEN
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_TRUE(result);
}