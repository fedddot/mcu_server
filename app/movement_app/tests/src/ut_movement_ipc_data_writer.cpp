#include <stdexcept>
#include <string>

#include "json/reader.h"
#include "json/value.h"

#include "gtest/gtest.h"

#include "ipc_instance.hpp"
#include "response_json_transformer.hpp"
#include "movement_ipc_data_writer.hpp"
#include "movement_manager_response.hpp"
#include "test_ipc_data_writer.hpp"

using namespace ipc;
using namespace manager;

TEST(ut_movement_ipc_data_writer, ctor_dtor_sanity) {
	// GIVEN
	const auto raw_data_writer = Instance<IpcDataWriter<RawData>>(
		new TestIpcDataWriter<RawData>(
			[](const RawData&) {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);

	// WHEN
	MovementIpcDataWriter *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new MovementIpcDataWriter(raw_data_writer));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_ipc_data_writer, write_sanity) {
	// GIVEN
	auto test_response = MovementManagerResponse {
		MovementManagerResponse::ResultCode::EXCEPTION,
		"test message"
	};
	
	const auto raw_data_writer = Instance<IpcDataWriter<RawData>>(
		new TestIpcDataWriter<RawData>(
			[test_response](const RawData& raw_data) {
				const auto response_transformer = ResponseJsonTransformer();
				Json::Value root;
				Json::Reader reader;
				ASSERT_TRUE(reader.parse(std::string(raw_data.begin(), raw_data.end()), std::ref(root), true));
				const auto received_response = response_transformer.json_value_to_response(root);
				ASSERT_EQ(test_response.code, received_response.code);
				ASSERT_TRUE(received_response.message.has_value());
				ASSERT_EQ(test_response.message.value(), received_response.message.value());
			}
		)
	);
	
	// WHEN
	auto instance = MovementIpcDataWriter(raw_data_writer);
	
	// THEN
	ASSERT_NO_THROW(instance.write(test_response));
}