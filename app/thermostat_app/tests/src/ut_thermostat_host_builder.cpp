#include <cstdint>
#include <stdexcept>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <vector>

#include "ipc_queue.hpp"
#include "ring_queue.hpp"
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
const auto HSIZE = std::size_t(2UL);
using ApiRequest = ThermostatApp<HSIZE>::ApiRequest;
using ApiResponse = ThermostatApp<HSIZE>::ApiResponse;

TEST(ut_thermostat_host_builder, ctor_dtor_sanity) {
	// WHEN
	ThermostatApp<HSIZE> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new ThermostatApp<HSIZE>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_thermostat_host_builder, build_sanity) {
	// GIVEN
	const auto queue_size = 10UL;
	auto queue = ipc::RingQueue<std::uint8_t, queue_size>();
	const auto package_size_retriever = [](const ipc::IpcQueue<std::uint8_t>&) -> std::size_t {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	const auto header_generator = [](const std::vector<std::uint8_t>&, const std::size_t&) -> std::vector<std::uint8_t> {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
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
	ThermostatApp<HSIZE> instance;
	instance
		.set_package_size_retriever(package_size_retriever)
		.set_api_request_parser(api_request_parser)
		.set_api_response_serializer(api_response_serializer)
		.set_header_generator(header_generator)
		.set_raw_data_writer(raw_data_writer)
		.set_raw_data_queue(&queue);

	// THEN
	ASSERT_NO_THROW(
		{
			instance.build();
		}
	);
}