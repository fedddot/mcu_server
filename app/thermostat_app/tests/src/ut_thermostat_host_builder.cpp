#include <cstdint>
#include <stdexcept>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <vector>

#include "ring_data_buffer.hpp"
#include "thermostat_host_builder.hpp"
#include "thermostat_controller.hpp"
#include "thermostat_service.hpp"

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
const auto L = 2UL;
using ApiRequest = ThermostatHostBuilder<L>::ApiRequest;
using ApiResponse = ThermostatHostBuilder<L>::ApiResponse;

TEST(ut_thermostat_host_builder, ctor_dtor_sanity) {
	// WHEN
	ThermostatHostBuilder<L> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new ThermostatHostBuilder<L>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_thermostat_host_builder, build_sanity) {
	// GIVEN
	const auto buffer_size = 10UL;
	auto raw_data_buffer = host_tools::RingDataBuffer<std::uint8_t, buffer_size>();
	const auto raw_data_writer = [](const std::vector<std::uint8_t>&) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	const auto api_request_parser = [](const std::vector<std::uint8_t>&) -> ApiRequest {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	const auto api_response_serializer = [](const ApiResponse&) -> std::vector<std::uint8_t> {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	auto controller = NiceMock<MockThermostatController>();
	auto service = ThermostatService(&controller);
	
	// WHEN
	ThermostatHostBuilder<L> instance;
	instance
		.set_raw_data_buffer(&raw_data_buffer)
		.set_api_request_parser(api_request_parser)
		.set_raw_data_writer(raw_data_writer)
		.set_api_response_serializer(api_response_serializer)
		.set_service(&service);

	// THEN
	ASSERT_NO_THROW(
		{
			const auto host = instance.build();
		}
	);
}