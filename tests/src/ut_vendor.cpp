#include <iostream>
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "gpio.hpp"
#include "gpio_test_data_creator.hpp"
#include "object.hpp"
#include "server.hpp"
#include "server_fixture.hpp"
#include "stepper_motor_test_data_creator.hpp"

using namespace server;
using namespace server_uts;
using namespace mcu_platform;
using namespace mcu_factory_uts;

TEST_F(ServerFixture, ctor_dtor_sanity) {
	// WHEN
	Server *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new Server(factory(), fail_report_creator()));
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

using CheckReportFunction = std::function<void(const Data&)>;
inline static void run_sanity_tc(const std::string& title, Server *instance_ptr, const Data& test_data, const CheckReportFunction& report_checker) {
	// WHEN
	std::unique_ptr<Data> report_ptr(nullptr);

	// THEN
	std::cout << "running TC: " << title << std::endl;
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(instance_ptr->run(test_data)));
	ASSERT_NE(nullptr, report_ptr);
	report_checker(*report_ptr);
}

TEST_F(ServerFixture, run_gpio_tasks_sanity) {
	// GIVEN
	auto check_report = [](const Data& data) {
		ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(data).access("result")).get());
	};
	auto check_gpio_state_report = [check_report](const Data& data, const Gpio::State& expected_state) {
		check_report(data);
		ASSERT_EQ(expected_state, static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get()));
	};

	const GpioId test_gpi_id(12);
	const GpioId test_gpo_id(13);
	
	using TestCase = std::pair<std::string, std::pair<Object, CheckReportFunction>>;
	using GpioDirection = typename Gpio::Direction;
	using GpioState = typename Gpio::State;

	const std::vector<TestCase> test_cases{
		{
			"gpi creation",
			{
				GpioTestDataCreator().create_gpio_data(test_gpi_id, GpioDirection::IN),
				check_report
			}
		},
		{
			"gpo creation",
			{
				GpioTestDataCreator().create_gpio_data(test_gpo_id, GpioDirection::OUT),
				check_report
			}
		},
		{
			"gpo set",
			{
				GpioTestDataCreator().set_gpio_data(test_gpo_id, GpioState::HIGH),
				check_report
			}
		},
		{
			"gpo get",
			{
				GpioTestDataCreator().get_gpio_data(test_gpo_id),
				[check_gpio_state_report](const Data& data) {
					check_gpio_state_report(data, GpioState::HIGH);
				}
			}
		},
		{
			"gpi get",
			{
				GpioTestDataCreator().get_gpio_data(test_gpi_id),
				[check_gpio_state_report](const Data& data) {
					check_gpio_state_report(data, GpioState::LOW);
				}
			}
		},
		{
			"gpi deletion",
			{
				GpioTestDataCreator().delete_gpio_data(test_gpi_id),
				check_report
			}
		},
		{
			"gpo deletion",
			{
				GpioTestDataCreator().delete_gpio_data(test_gpo_id),
				check_report
			}
		}
	};
	Server instance(factory(), fail_report_creator());

	// THEN
	for (const auto& [title, tc_data]: test_cases) {
		run_sanity_tc(title, &instance, tc_data.first, tc_data.second);
	}
}

TEST_F(ServerFixture, run_motor_tasks_sanity) {
	// GIVEN
	using GpioDirection = typename Gpio::Direction;
	using GpioState = typename Gpio::State;
	using TestCase = std::pair<std::string, std::pair<Object, CheckReportFunction>>;

	auto check_report = [](const Data& data) {
		ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(data).access("result")).get());
	};

	const States test_states {
		State {
			{Shoulder::IN0, GpioState::HIGH},
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::LOW}
		},
		State {
			{Shoulder::IN0, GpioState::LOW},
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::HIGH},
			{Shoulder::IN3, GpioState::LOW}
		}
	};

	const StepperId test_stepper_x(12);
	const Shoulders stepper_x_shoulders {
		{Shoulder::IN0, 0},
		{Shoulder::IN1, 1},
		{Shoulder::IN2, 2},
		{Shoulder::IN3, 3}
	};

	const StepperId test_stepper_y(13);
	const Shoulders stepper_y_shoulders {
		{Shoulder::IN0, 4},
		{Shoulder::IN1, 5},
		{Shoulder::IN2, 6},
		{Shoulder::IN3, 7}
	};
	

	const std::vector<TestCase> test_cases {
		{
			"motor x creation",
			{
				StepperMotorTestDataCreator().create_data(test_stepper_x, stepper_x_shoulders, test_states),
				check_report
			}
		},
		{
			"motor x steps",
			{
				StepperMotorTestDataCreator().steps_data(
					Steps {
						.stepper_id = test_stepper_x,
						.direction = MotorDirection::CCW,
						.steps_number = 100,
						.step_duration_ms = 10
					}
				),
				check_report
			}
		},
		{
			"motor y creation",
			{
				StepperMotorTestDataCreator().create_data(test_stepper_y, stepper_y_shoulders, test_states),
				check_report
			}
		},
		{
			"motor y steps",
			{
				StepperMotorTestDataCreator().steps_data(
					Steps {
						.stepper_id = test_stepper_y,
						.direction = MotorDirection::CW,
						.steps_number = 50,
						.step_duration_ms = 3
					}
				),
				check_report
			}
		},
		{
			"steps sequence",
			{
				StepperMotorTestDataCreator().steps_sequence_data(
					StepsSequence {
						Steps {
							.stepper_id = test_stepper_x,
							.direction = MotorDirection::CCW,
							.steps_number = 100,
							.step_duration_ms = 10
						},
						Steps {
							.stepper_id = test_stepper_y,
							.direction = MotorDirection::CW,
							.steps_number = 50,
							.step_duration_ms = 3
						}
					}
				),
				check_report
			}
		},
		{
			"motor y deletion",
			{
				StepperMotorTestDataCreator().delete_data(test_stepper_y),
				check_report
			}
		},
		{
			"motor x deletion",
			{
				StepperMotorTestDataCreator().delete_data(test_stepper_x),
				check_report
			}
		}
	};
	Server instance(factory(), fail_report_creator());

	// THEN
	for (const auto& [title, tc_data]: test_cases) {
		run_sanity_tc(title, &instance, tc_data.first, tc_data.second);
	}
}