#ifndef STEPPER_MOTOR_TEST_DATA_CREATOR_HPP
#define STEPPER_MOTOR_TEST_DATA_CREATOR_HPP

#include <string>

#include "array.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor_tasks_factory.hpp"
#include "string.hpp"

namespace mcu_factory_uts {
	class StepperMotorTestDataCreator {
	public:
		using StepperId = int;
		using GpioId = int;
		using Shoulder = typename mcu_platform::StepperMotor<GpioId>::Shoulder;
		using Shoulders = typename mcu_platform::StepperMotor<GpioId>::Shoulders;
		using States = typename mcu_platform::StepperMotor<GpioId>::States;
		using Steps = typename mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>::Steps;
		using StepsSequence = typename mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>::StepsSequence;
		using TaskType = typename mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>::TaskType;

		StepperMotorTestDataCreator() = default;
		StepperMotorTestDataCreator(const StepperMotorTestDataCreator&) = delete;
		StepperMotorTestDataCreator& operator=(const StepperMotorTestDataCreator&) = delete;

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
			create_data.add("domain", String("stepper_motor"));
			return create_data;
		}

		const server::Object steps_data(const Steps& steps) const {
			using namespace server;
			Object steps_data;
			steps_data.add("task_type", Integer(static_cast<int>(TaskType::STEPS)));
			steps_data.add("stepper_id", Integer(steps.stepper_id));
			steps_data.add("direction", Integer(static_cast<int>(steps.direction)));
			steps_data.add("steps_number", Integer(steps.steps_number));
			steps_data.add("step_duration_ms", Integer(steps.step_duration_ms));
			steps_data.add("domain", String("stepper_motor"));
			return steps_data;
		}

		const server::Object steps_sequence_data(const StepsSequence& sequence) const {
			using namespace server;
			Array sequence_array;
			for (auto steps: sequence) {
				auto steps_object(steps_data(steps));
				steps_object.remove("task_type");
				sequence_array.push_back(steps_object);
			}
			Object steps_sequence_data;
			steps_sequence_data.add("task_type", Integer(static_cast<int>(TaskType::STEPS_SEQUENCE)));
			steps_sequence_data.add("sequence", sequence_array);
			steps_sequence_data.add("domain", String("stepper_motor"));
			return steps_sequence_data;
		}

		const server::Object delete_data(const StepperId& stepper_id) const {
			using namespace server;
			Object delete_data;
			delete_data.add("task_type", Integer(static_cast<int>(TaskType::DELETE_STEPPER_MOTOR)));
			delete_data.add("stepper_id", Integer(stepper_id));
			delete_data.add("domain", String("stepper_motor"));
			return delete_data;
		}
	private:
		static std::string shoulder_to_str(const Shoulder& shoulder) {
			const std::string prefix("IN");
			return prefix + std::to_string(static_cast<int>(shoulder));
		}
	};
}

#endif