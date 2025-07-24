#include <stdexcept>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "thermostat_host_builder.hpp"
#include "thermostat_controller.hpp"

using namespace host;
using namespace service;
using namespace testing;

class MockThermostatController : public ThermostatController {
public:
    MOCK_METHOD(void, set_relay_state, (const bool state), (override));
	MOCK_METHOD(double, read_temperature, (), (const, override));
	MOCK_METHOD(TaskGuard *, schedule_task, (const Task& task, const std::size_t period_ms), (override));
};

class MockTaskGuard : public ThermostatController::TaskGuard {
	public:
    MOCK_METHOD(void, unschedule, (), (override));
};

using RawData = std::string;

TEST(ut_thermostat_host_builder, ctor_dtor_sanity) {
	// WHEN
	ThermostatHostBuilder<RawData> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new ThermostatHostBuilder<RawData>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

// TEST(ut_thermostat_host_builder, build_sanity) {
// 	// GIVEN
// 	const auto raw_data_reader = ThermostatHostBuilder<RawData>::RawDataReaderInstance(
// 		new TestIpcDataReader<RawData>(
// 			[]() -> std::optional<ipc::Instance<RawData>> {
// 				throw std::runtime_error("NOT IMPLEMENTED");
// 			}
// 		)
// 	);
// 	const auto raw_data_writer = ThermostatHostBuilder<RawData>::RawDataWriterInstance(
// 		new TestIpcDataWriter<RawData>(
// 			[](const RawData&) {
// 				throw std::runtime_error("NOT IMPLEMENTED");
// 			}
// 		)
// 	);
// 	const auto api_request_parser = [](const RawData&) -> ipc::Instance<ThermostatHostBuilder<RawData>::ApiRequest> {
// 		throw std::runtime_error("NOT IMPLEMENTED");
// 	};
// 	const auto api_response_serializer = [](const ThermostatHostBuilder<RawData>::ApiResponse&) -> RawData {
// 		throw std::runtime_error("NOT IMPLEMENTED");
// 	};
// 	auto controller = NiceMock<MockThermostatController>();
	
// 	// WHEN
// 	ThermostatHostBuilder<RawData> instance;
// 	instance
// 		.set_raw_data_reader(raw_data_reader)
// 		.set_api_request_parser(api_request_parser)
// 		.set_raw_data_writer(raw_data_writer)
// 		.set_api_response_serializer(api_response_serializer)
// 		.set_controller(&controller);

// 	// THEN
// 	ASSERT_NO_THROW(
// 		{
// 			const auto host = instance.build();
// 		}
// 	);
// }