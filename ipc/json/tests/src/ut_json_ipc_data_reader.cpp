#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/value.h"

#include "ipc_data_reader.hpp"
#include "clonable_ipc_data_reader.hpp"
#include "json_ipc_data_reader.hpp"

using namespace ipc;

using TestIpcData = std::string;
using RawData = typename JsonIpcDataReader<TestIpcData>::RawData;

class TestRawReader: public ClonableIpcDataReader<RawData> {
public:
	TestRawReader(const std::optional<RawData>& data_opt) {
		m_data_option = data_opt;
	}
	TestRawReader(const TestRawReader&) = default;
	TestRawReader& operator=(const TestRawReader&) = default;
	
	std::optional<RawData> read() override {
		return m_data_option;
	}
	IpcDataReader<RawData> *clone() const override {
		return new TestRawReader(*this);
	}
private:
	std::optional<RawData> m_data_option;
};

TEST(ut_json_ipc_data_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto raw_data_reader = TestRawReader(std::optional<RawData>());
	auto data_parser_from_json = [](const Json::Value& val) -> TestIpcData {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	// WHEN
	JsonIpcDataReader<TestIpcData> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new JsonIpcDataReader<TestIpcData>(raw_data_reader, data_parser_from_json));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

// static RawData serialize_request(const TestIpcData& request);

// TEST(ut_json_ipc_data_reader, read_sanity) {
// 	// GIVEN
// 	const auto request = TestIpcData("request");
// 	const auto request_json_raw = serialize_request(request);
	
// 	// WHEN
// 	auto raw_reader = TestRawReader();
// 	raw_reader.update_data(request_json_raw);
// 	auto instance = JsonIpcDataReader<TestIpcData>(
// 		SharedIpcDataReader<RawData>(new TestRawReader(raw_reader)),
// 		[](const RawData&) -> TestIpcData {
// 			throw std::runtime_error("NOT IMPLEMENTED");
// 		}
// 	);
// 	auto result = std::optional<TestIpcData>();

// 	// THEN
// 	ASSERT_NO_THROW(result = instance.read());
// 	ASSERT_TRUE(result);
// }