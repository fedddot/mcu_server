#include <stdexcept>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ipc_data.hpp"
#include "ipc_instance.hpp"
#include "manager_instance.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"
#include "thermostat_host_builder.hpp"

using namespace ipc;
using namespace manager;
using namespace host;
using namespace testing;

class MockThermostatManagerController : public ThermostatManagerController {
public:
    MOCK_METHOD(void, set_relay_state, (const bool state), (override));
	MOCK_METHOD(double, read_temperature, (), (const, override));
	MOCK_METHOD(manager::Instance<TaskGuard>, schedule_task, (const Task& task, const std::size_t period_ms), (override));
};

class MockTaskGuard : public ThermostatManagerController::TaskGuard {
	public:
    MOCK_METHOD(void, unschedule, (), (override));
};

TEST(ut_thermostat_host_builder, ctor_dtor_sanity) {
	// WHEN
	ThermostatHostBuilder *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new ThermostatHostBuilder()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_thermostat_host_builder, build_sanity) {
	// GIVEN
	const auto raw_data_reader = ThermostatHostBuilder::RawDataReaderInstance(
		new TestIpcDataReader<RawData>(
			[]() -> std::optional<ipc::Instance<RawData>> {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto raw_data_writer = ThermostatHostBuilder::RawDataWriterInstance(
		new TestIpcDataWriter<RawData>(
			[](const RawData&) {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto api_request_parser = [](const RawData&) -> ipc::Instance<ThermostatHostBuilder::ApiRequest> {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	const auto api_response_serializer = [](const ThermostatHostBuilder::ApiResponse&) -> RawData {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	auto controller = NiceMock<MockThermostatManagerController>();
	
	// WHEN
	ThermostatHostBuilder instance;
	instance
		.set_raw_data_reader(raw_data_reader)
		.set_api_request_parser(api_request_parser)
		.set_raw_data_writer(raw_data_writer)
		.set_api_response_serializer(api_response_serializer)
		.set_controller(&controller);

	// THEN
	ASSERT_NO_THROW(
		{
			const auto host = instance.build();
		}
	);
}