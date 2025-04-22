#include <cstddef>
#include <stdexcept>

#include "gtest/gtest.h"

#include "stepper_host.hpp"
#include "stepper_ipc_data_infra.hpp"
#include "stepper_motor_data.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"
#include "test_stepper_motor.hpp"

using namespace ipc;
using namespace manager;
using namespace host;

TEST(ut_stepper_host, ctor_dtor_sanity) {
	// GIVEN
	const auto raw_data_writer = TestIpcDataWriter<RawData>(
		[](const RawData& raw_data) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	const auto raw_data_reader = TestIpcDataReader<RawData>(
		[]() -> std::optional<RawData> {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	auto stepper_motor_creator = []() -> StepperMotor * {
		return new manager_tests::TestStepperMotor(
			[](const Direction&) {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		);
	};
	auto delay_generator = [](const std::size_t&) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	StepperHost *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new StepperHost(raw_data_reader, raw_data_writer, stepper_motor_creator, delay_generator));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}