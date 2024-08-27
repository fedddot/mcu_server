#ifndef STEPPER_MOTOR_TASKS_FACTORY_FIXTURE_HPP
#define STEPPER_MOTOR_TASKS_FACTORY_FIXTURE_HPP

#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "array.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor_tasks_factory.hpp"
#include "default_stepper_motor_tasks_data_retriever.hpp"
#include "test_platform.hpp"

namespace mcu_factory_uts {
	class StepperMotorTasksFactoryFixture: public testing::Test {
	public:
		using GpioId = int;
		using StepperId = int;
		using TestFactory = mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>;
		using MotorInventory = typename TestFactory::MotorInventory;
		using GpioState = typename mcu_platform::Gpio::State;
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using DataRetriever = typename TestFactory::DataRetriever;
		using FactoryPlatform = typename TestFactory::FactoryPlatform;
		using ResultReporter = typename TestFactory::ResultReporter;

		using States = typename mcu_platform::StepperMotor<GpioId>::States;
		using Direction = typename mcu_platform::StepperMotor<GpioId>::Direction;
		using Shoulders = typename mcu_platform::StepperMotor<GpioId>::Shoulders;
		using TaskType = typename TestFactory::TaskType;
		using Shoulder = typename mcu_platform::StepperMotor<GpioId>::Shoulder;

		StepperMotorTasksFactoryFixture();
		StepperMotorTasksFactoryFixture(const StepperMotorTasksFactoryFixture&) = delete;
		StepperMotorTasksFactoryFixture& operator=(const StepperMotorTasksFactoryFixture&) = delete;
		
		MotorInventory *inventory() const {
			return &m_inventory;
		}

		FactoryPlatform *platform() const {
			return &m_platform;
		}

		const DataRetriever& data_retriever() const {
			return *m_data_retriever;
		}

		const ResultReporter& result_reporter() const {
			throw std::runtime_error("NOT IMPLEMENTED");
		}

		const server::Object create_data(const StepperId& stepper_id, const Shoulders& shoulders, const States& states) const {
			using namespace server;
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

		const server::Object steps_data(const StepperId& stepper_id, const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {
			using namespace server;
			Object steps_data;
			steps_data.add("task_type", Integer(static_cast<int>(TaskType::STEPS)));
			steps_data.add("stepper_id", Integer(stepper_id));
			steps_data.add("direction", Integer(static_cast<int>(direction)));
			steps_data.add("steps_number", Integer(steps_num));
			steps_data.add("step_duration_ms", Integer(step_duration_ms));
			return steps_data;
		}

		// const server::Object steps_sequence_data(const StepsSequence& sequence) const {
		// 	using namespace server;
		// 	Array sequence_array;
		// 	for (auto steps: sequence) {
		// 		Object steps_object;
		// 		steps_object.add("stepper_id", Integer(static_cast<int>(steps.stepper_id())));
		// 		steps_object.add("direction", Integer(static_cast<int>(steps.direction())));
		// 		steps_object.add("steps_number", Integer(static_cast<int>(steps.steps_number())));
		// 		steps_object.add("step_duration_ms", Integer(static_cast<int>(steps.step_duration_ms())));
		// 		sequence_array.push_back(steps_object);
		// 	}
		// 	Object steps_sequence_data;
		// 	steps_sequence_data.add("task_type", Integer(static_cast<int>(TaskType::STEPS_SEQUENCE)));
		// 	steps_sequence_data.add("sequence", sequence_array);
		// 	return steps_sequence_data;
		// }

		const server::Object delete_data(const StepperId& stepper_id) const {
			using namespace server;
			Object delete_data;
			delete_data.add("task_type", Integer(static_cast<int>(TaskType::DELETE_STEPPER_MOTOR)));
			delete_data.add("stepper_id", Integer(stepper_id));
			return delete_data;
		}
	private:
		mutable MotorInventory m_inventory;
		mutable mcu_platform_uts::TestPlatform<GpioId> m_platform;
		std::unique_ptr<DataRetriever> m_data_retriever;
		
		static std::string shoulder_to_str(const Shoulder& shoulder) {
			const std::string prefix("IN");
			return prefix + std::to_string(static_cast<int>(shoulder));
		}
	};

	inline StepperMotorTasksFactoryFixture::StepperMotorTasksFactoryFixture() {
		using namespace mcu_factory;		
		m_data_retriever = std::unique_ptr<DataRetriever>(new DefaultStepperMotorDataRetriever<TaskType, GpioId, StepperId>());
	}
}

#endif