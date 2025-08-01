#include <cstdint>
#include <stdexcept>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ipc_queue.hpp"
#include "ring_queue.hpp"
#include "thermostat_app.hpp"
#include "thermostat_controller.hpp"
#include "package_utils.hpp"
#include "proto_thermostat_api_request_serializer.hpp"

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

const auto HSIZE = std::size_t(2UL);
using ApiRequest = ThermostatApp<HSIZE>::ApiRequest;
using ApiResponse = ThermostatApp<HSIZE>::ApiResponse;
static void write_request_to_queue(ipc::IpcQueue<std::uint8_t> *queue, const ApiRequest& request);

TEST(ut_thermostat_app, run_once_sanity) {
	// GIVEN
	const auto test_request = ApiRequest(ApiRequest::RequestType::GET_TEMP);
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

	// WHEN
	ThermostatApp<HSIZE> instance(
		package_size_retriever,
		api_request_parser,
		api_response_serializer,
		header_generator,
		raw_data_writer,
		&queue,
		&controller
	);
	write_request_to_queue(&queue, test_request);

	// THEN
	ASSERT_NO_THROW(instance.run_once());
}

inline void write_request_to_queue(ipc::IpcQueue<std::uint8_t> *queue, const ApiRequest& request) {
	const auto serializer = ipc::ApiRequestSerializer();
	auto raw_data = serializer(request);
	const auto encoded_size = ipc::serialize_package_size(raw_data.size(), HSIZE);
	for (const auto& byte : encoded_size) {
		queue->enqueue(byte);
	}
	for (const auto& byte : raw_data) {
		queue->enqueue(byte);
	}
}