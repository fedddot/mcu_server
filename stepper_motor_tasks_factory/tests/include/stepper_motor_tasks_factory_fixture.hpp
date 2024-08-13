#ifndef STEPPER_MOTOR_TASKS_FACTORY_FIXTURE_HPP
#define STEPPER_MOTOR_TASKS_FACTORY_FIXTURE_HPP

#include <memory>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "array.hpp"
#include "default_stepper_motor_data_parser.hpp"
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
		using TestDataParser = typename TestFactory::DataParser;
		using States = mcu_platform::StepperMotor<GpioId>::States;
		using Shoulders = mcu_platform::StepperMotor<GpioId>::Shoulders;
		using TaskType = typename TestFactory::TaskType;
		using Shoulder = mcu_platform::StepperMotor<GpioId>::Shoulder;

		StepperMotorTasksFactoryFixture();
		StepperMotorTasksFactoryFixture(const StepperMotorTasksFactoryFixture&) = delete;
		StepperMotorTasksFactoryFixture& operator=(const StepperMotorTasksFactoryFixture&) = delete;
		
		StepperMotorInventory *inventory() const {
			return &m_inventory;
		}

		const TestDataParser& data_parser() const {
			return *m_data_parser;
		}

		const mcu_server::Object create_data(const StepperId& stepper_id, const Shoulders& shoulders, const States& states) const {
			using namespace mcu_server;
			Object shoulders_data;
			for (auto [shoulder, gpio_id]: shoulders) {
				shoulders_data.add(shoulder_to_str(shoulder), Integer(static_cast<int>(gpio_id)));
			}

			Array states_data;
			for (auto state: states) {
				Object shoulders_states;
				for (auto [shoulder, gpio_state]: state) {
					shoulders_states.add(shoulder_to_str(shoulder),	Integer(static_cast<int>(gpio_state)));
				}
				states_data.push_back(shoulders_states);
			}

			Object create_data;
			create_data.add("task_type", Integer(static_cast<int>(TaskType::CREATE_STEPPER_MOTOR)));
			create_data.add("shoulders", shoulders_data);
			create_data.add("states", states_data);
			create_data.add("stepper_id", Integer(stepper_id));
			return create_data;
		}
	private:
		mutable StepperMotorInventory m_inventory;
		std::unique_ptr<TestDataParser> m_data_parser;
		
		static Shoulder str_to_shoulder(const std::string& shoulder_str) {
			const std::string prefix("IN");
			auto prefix_pos = shoulder_str.find(prefix);
			if (0 != prefix_pos) {
				throw std::invalid_argument("shoulder tag doesn't start with " + prefix);
			}
			auto shoulder_number = std::stoi(std::string(shoulder_str.begin() + prefix.size(), shoulder_str.end()));
			return static_cast<Shoulder>(shoulder_number);
		}

		static std::string shoulder_to_str(const Shoulder& shoulder) {
			const std::string prefix("IN");
			return prefix + std::to_string(static_cast<int>(shoulder));
		}
	};

	inline StepperMotorTasksFactoryFixture::StepperMotorTasksFactoryFixture() {
		using namespace mcu_factory;		
		m_data_parser = std::unique_ptr<TestDataParser>(new DefaultStepperMotorDataParser());
	}
}

#endif