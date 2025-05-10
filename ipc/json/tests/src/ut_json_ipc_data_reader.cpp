#include <optional>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/config.h"
#include "json/writer.h"
#include "json/value.h"

#include "ipc_clonable.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data.hpp"
#include "ipc_instance.hpp"
#include "json_ipc_data_reader.hpp"

using namespace ipc;

using TestIpcData = std::string;

class TestRawReader: public IpcDataReader<RawData>, public Clonable<IpcDataReader<RawData>> {
public:
	TestRawReader(const std::optional<Instance<RawData>>& data_opt): m_data_option(data_opt) {

	}
	TestRawReader(const TestRawReader&) = default;
	TestRawReader& operator=(const TestRawReader&) = default;
	
	std::optional<Instance<RawData>> read() override {
		return m_data_option;
	}
	IpcDataReader<RawData> *clone() const override {
		return new TestRawReader(*this);
	}
private:
	std::optional<Instance<RawData>> m_data_option;
};

static RawData serialize_ipc_data(const TestIpcData& ipc_data);
static Instance<TestIpcData> parse_ipc_data(const Json::Value& json_data);

TEST(ut_json_ipc_data_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto raw_data_reader = TestRawReader(std::nullopt);
	
	// WHEN
	JsonIpcDataReader<TestIpcData> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new JsonIpcDataReader<TestIpcData>(raw_data_reader, parse_ipc_data));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_json_ipc_data_reader, read_sanity) {
	// GIVEN
	const auto test_ipc_data = TestIpcData("test_ipc_data");
	
	// WHEN
	const auto raw_test_ipc_data = Instance<RawData>(new RawData(serialize_ipc_data(test_ipc_data)));
	const auto raw_data_reader = TestRawReader(raw_test_ipc_data);
	auto instance = JsonIpcDataReader<TestIpcData>(raw_data_reader, parse_ipc_data);
	auto read_result = std::optional<Instance<TestIpcData>>();

	// THEN
	ASSERT_NO_THROW(read_result = instance.read());
	ASSERT_TRUE(read_result);
	ASSERT_EQ(test_ipc_data, read_result->get());
}

TEST(ut_json_ipc_data_reader, read_missing_raw_data_negative) {
	// WHEN
	const auto raw_data_reader = TestRawReader(std::optional<Instance<RawData>>());
	auto instance = JsonIpcDataReader<TestIpcData>(raw_data_reader, parse_ipc_data);
	auto read_result = std::optional<Instance<TestIpcData>>();

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

inline Instance<TestIpcData> parse_ipc_data(const Json::Value& json_data) {
	if (!json_data.isString()) {
		throw std::invalid_argument("json data is expected to be a string");
	}
	return Instance<TestIpcData>(new TestIpcData(json_data.asString()));
}