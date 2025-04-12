#include "json/config.h"
#include "json/writer.h"
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/value.h"

#include "ipc_data_writer.hpp"
#include "clonable_ipc_data_writer.hpp"
#include "json_ipc_data_writer.hpp"

using namespace ipc;

using TestIpcData = std::string;
using RawData = typename JsonIpcDataWriter<TestIpcData>::RawData;

static RawData serialize_ipc_data(const TestIpcData& ipc_data);
static Json::Value transform_ipc_data(const TestIpcData& data);

class TestRawWriter: public ClonableIpcDataWriter<RawData> {
public:
	TestRawWriter(const std::function<void(const RawData&)>& test_action): m_test_action(test_action) {
		if (!m_test_action) {
			throw std::invalid_argument("invalid action received");
		}
	}
	TestRawWriter(const TestRawWriter&) = default;
	TestRawWriter& operator=(const TestRawWriter&) = default;
	
	void write(const RawData& data) const override {
		m_test_action(data);
	}
	IpcDataWriter<RawData> *clone() const override {
		return new TestRawWriter(*this);
	}
private:
	std::function<void(const RawData&)> m_test_action;
};

TEST(ut_json_ipc_data_writer, ctor_dtor_sanity) {
	// GIVEN
	const auto raw_data_writer = TestRawWriter(
		[](const RawData&) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);

	// WHEN
	JsonIpcDataWriter<TestIpcData> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new JsonIpcDataWriter<TestIpcData>(raw_data_writer, transform_ipc_data));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_json_ipc_data_writer, write_sanity) {
	// GIVEN
	const auto test_ipc_data = TestIpcData("test_ipc_data");
	const auto raw_test_ipc_data = serialize_ipc_data(test_ipc_data);
	
	// WHEN
	const auto raw_data_writer = TestRawWriter(
		[raw_test_ipc_data](const RawData& data) {
			ASSERT_EQ(raw_test_ipc_data, data);
		}
	);
	auto instance = JsonIpcDataWriter<TestIpcData>(raw_data_writer, transform_ipc_data);
	
	// THEN
	ASSERT_NO_THROW(instance.write(test_ipc_data));
}

inline RawData serialize_ipc_data(const TestIpcData& ipc_data) {
	const auto json_val = Json::String(ipc_data);
	const auto writer_builder = Json::StreamWriterBuilder();
    const auto serial_str = Json::writeString(writer_builder, json_val);
    return RawData(serial_str.begin(), serial_str.end());
}

inline Json::Value transform_ipc_data(const TestIpcData& data) {
	return Json::String(data);
}