#include "json/config.h"
#include "json/writer.h"
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

static RawData serialize_ipc_data(const TestIpcData& ipc_data);
static TestIpcData parse_ipc_data(const Json::Value& json_data);

TEST(ut_json_ipc_data_reader, read_sanity) {
	// GIVEN
	const auto test_ipc_data = TestIpcData("test_ipc_data");
	
	// WHEN
	const auto raw_test_ipc_data = serialize_ipc_data(test_ipc_data);
	const auto raw_data_reader = TestRawReader(std::optional<RawData>(raw_test_ipc_data));
	auto instance = JsonIpcDataReader<TestIpcData>(raw_data_reader, parse_ipc_data);
	auto read_result = std::optional<TestIpcData>();

	// THEN
	ASSERT_NO_THROW(read_result = instance.read());
	ASSERT_TRUE(read_result);
	ASSERT_EQ(test_ipc_data, *read_result);
}

TEST(ut_json_ipc_data_reader, read_missing_raw_data_negative) {
	// WHEN
	const auto raw_data_reader = TestRawReader(std::optional<RawData>());
	auto instance = JsonIpcDataReader<TestIpcData>(raw_data_reader, parse_ipc_data);
	auto read_result = std::optional<TestIpcData>();

	// THEN
	ASSERT_NO_THROW(read_result = instance.read());
	ASSERT_FALSE(read_result);
}

inline RawData serialize_ipc_data(const TestIpcData& ipc_data) {
	const auto json_val = Json::String(ipc_data);
	const auto writer_builder = Json::StreamWriterBuilder();
    const auto serial_str = Json::writeString(writer_builder, json_val);
    return RawData(serial_str.begin(), serial_str.end());
}

inline TestIpcData parse_ipc_data(const Json::Value& json_data) {
	if (!json_data.isString()) {
		throw std::invalid_argument("json data is expected to be a string");
	}
	return TestIpcData(json_data.asString());
}