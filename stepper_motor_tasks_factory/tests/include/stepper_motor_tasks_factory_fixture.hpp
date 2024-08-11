#ifndef STEPPER_MOTOR_TASKS_FACTORY_FIXTURE_HPP
#define STEPPER_MOTOR_TASKS_FACTORY_FIXTURE_HPP

#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "array.hpp"
#include "data.hpp"
#include "custom_parser.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor_tasks_factory.hpp"

namespace mcu_factory_uts {
	class StepperMotorTasksFactoryFixture: public testing::Test {
	public:
		using GpioId = int;
		using StepperId = int;
		using TestFactory = mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>;
		using StepperMotorInventory = typename TestFactory::StepperMotorInventory;
		using TaskTypeParser = typename TestFactory::TaskTypeParser;
		using StepperIdParser = typename TestFactory::StepperIdParser;
		using StatesParser = typename TestFactory::StatesParser;
		using ShouldersParser = typename TestFactory::ShouldersParser;

		StepperMotorTasksFactoryFixture();
		StepperMotorTasksFactoryFixture(const StepperMotorTasksFactoryFixture&) = delete;
		StepperMotorTasksFactoryFixture& operator=(const StepperMotorTasksFactoryFixture&) = delete;
		
		StepperMotorInventory *inventory() const {
			return &m_inventory;
		}

		const TaskTypeParser& task_type_parser() const {
			return *m_task_type_parser;
		}

		const StepperIdParser& stepper_id_parser() const {
			return *m_stepper_id_parser;
		}

		const StatesParser& states_parser() const {
			return *m_states_parser;
		}

		const ShouldersParser& shoulders_parser() const {
			return *m_shoulders_parser;
		}
	private:
		mutable StepperMotorInventory m_inventory;
		std::unique_ptr<TaskTypeParser> m_task_type_parser;
		std::unique_ptr<StepperIdParser> m_stepper_id_parser;
		std::unique_ptr<StatesParser> m_states_parser;
		std::unique_ptr<ShouldersParser> m_shoulders_parser;

		using Shoulder = mcu_platform::StepperMotor<GpioId>::Shoulder;
		static Shoulder cast_shoulder(const std::string& shoulder_str) {
			const std::string prefix("IN");
			auto prefix_pos = shoulder_str.find(prefix);
			if (0 != prefix_pos) {
				throw std::invalid_argument("shoulder tag doesn't start with " + prefix);
			}
			auto shoulder_number = std::stoi(std::string(shoulder_str.begin() + prefix.size(), shoulder_str.end()));
			switch (shoulder_number) {
			case 0:
				return Shoulder::IN0;
			case 1:
				return Shoulder::IN1;
			case 2:
				return Shoulder::IN2;
			case 3:
				return Shoulder::IN3;
			default:
				throw std::invalid_argument("invalid shoulder number");
			}
		}
	};

	inline StepperMotorTasksFactoryFixture::StepperMotorTasksFactoryFixture() {
		using namespace mcu_factory;
		using namespace mcu_server;
		using namespace mcu_server_utl;
		using namespace mcu_platform;
		using TaskType = typename TestFactory::TaskType;
		
		m_task_type_parser = std::unique_ptr<TaskTypeParser>(
			new CustomParser<TaskType(const Data&)>(
				[](const Data& data) {
					return static_cast<TaskType>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
				}
			)
		);
		
		m_stepper_id_parser = std::unique_ptr<StepperIdParser>(
			new CustomParser<StepperId(const Data&)>(
				[](const Data& data) {
					return static_cast<StepperId>(Data::cast<Integer>(Data::cast<Object>(data).access("stepper_id")).get());
				}
			)
		);

		using States = mcu_platform::StepperMotor<GpioId>::States;
		m_states_parser = std::unique_ptr<StatesParser>(
			new CustomParser<States(const Data&)>(
				[](const Data& data) {
					States states;
					Data::cast<Array>(Data::cast<Object>(data).access("states")).for_each(
						[&states](int index, const Data& state_data) {
							using State = typename mcu_platform::StepperMotor<GpioId>::State;
							using GpioState = typename Gpio::State;
							State state;
							Data::cast<Object>(state_data).for_each(
								[&state](const std::string& shoulder, const Data& gpio_state) {
									state.insert({cast_shoulder(shoulder), static_cast<GpioState>(Data::cast<Integer>(gpio_state).get())});
								}
							);
							states.push_back(state);
						}
					);
					return states;
				}
			)
		);

		using Shoulders = mcu_platform::StepperMotor<GpioId>::Shoulders;
		m_shoulders_parser = std::unique_ptr<ShouldersParser>(
			new CustomParser<Shoulders(const Data&)>(
				[](const Data& data) {
					Shoulders shoulders;
					Data::cast<Object>(Data::cast<Object>(data).access("shoulders")).for_each(
						[&shoulders](const std::string& shoulder, const Data& gpio_id) {
							shoulders.insert(
								{
									cast_shoulder(shoulder),
									static_cast<GpioId>(Data::cast<Integer>(gpio_id).get())
								}
							);
						}
					);
					return shoulders;
				}
			)
		);
	}
}

#endif