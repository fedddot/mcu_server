#ifndef	DEFAULT_STEPPER_MOTOR_DATA_RETRIEVER_HPP
#define	DEFAULT_STEPPER_MOTOR_DATA_RETRIEVER_HPP

#include "data.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_tasks_data_retriever.hpp"

namespace mcu_factory {
	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	class DefaultStepperMotorDataRetriever: public StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id> {
	public:
		using StepperMotorDirection = typename StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::StepperMotorDirection;
		using Shoulders = typename StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::Shoulders;
		using States = typename StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::States;
		
		DefaultStepperMotorDataRetriever() = default;
		DefaultStepperMotorDataRetriever(const DefaultStepperMotorDataRetriever& other) = default;
		DefaultStepperMotorDataRetriever& operator=(const DefaultStepperMotorDataRetriever& other) = delete;

		DefaultStepperMotorDataRetriever *clone() const override;

		bool is_stepper_motor_task_creatable(const server::Data& data) const override;
		Ttask_type retrieve_task_type(const server::Data& data) const override;
		Tgpio_id retrieve_gpio_id(const server::Data& data) const override;
		Tstepper_id retrieve_stepper_id(const server::Data& data) const override;
		StepperMotorDirection retrieve_dir(const server::Data& data) const override;
		unsigned int retrieve_step_duration(const server::Data& data) const override;
		unsigned int retrieve_steps_number(const server::Data& data) const override;
		Shoulders retrieve_shoulders(const server::Data& data) const override;
		States retrieve_states(const server::Data& data) const override;
		
		StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id> *clone() const override;
	private:
		using Shoulder = typename mcu_platform::StepperMotor<int>::Shoulder;
		using State = typename mcu_platform::StepperMotor<int>::State;
		static Shoulder str_to_shoulder(const std::string& shoulder_str);
	};

	// template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	// inline StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id> *DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::clone() const {
	// 	return new DefaultStepperMotorDataRetriever(*this);
	// }

	// template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	// inline typename DefaultStepperMotorDataRetriever::TaskType DefaultStepperMotorDataRetriever::parse_task_type(const server::Data& data) const {
	// 	using namespace server;
	// 	return static_cast<TaskType>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
	// }

	// template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	// inline int DefaultStepperMotorDataRetriever::parse_stepper_id(const server::Data& data) const {
	// 	using namespace server;
	// 	return Data::cast<Integer>(Data::cast<Object>(data).access("stepper_id")).get();
	// }

	// template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	// inline typename DefaultStepperMotorDataRetriever::Duration DefaultStepperMotorDataRetriever::parse_step_duration(const server::Data& data) const {
	// 	using namespace server;
	// 	return static_cast<Duration>(Data::cast<Integer>(Data::cast<Object>(data).access("step_duration_ms")).get());
	// }

	// template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	// inline typename DefaultStepperMotorDataRetriever::StepsNumber DefaultStepperMotorDataRetriever::parse_steps_number(const server::Data& data) const {
	// 	using namespace server;
	// 	return static_cast<StepsNumber>(Data::cast<Integer>(Data::cast<Object>(data).access("steps_number")).get());
	// }

	// template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	// inline typename DefaultStepperMotorDataRetriever::Direction DefaultStepperMotorDataRetriever::parse_steps_direction(const server::Data& data) const {
	// 	using namespace server;
	// 	return static_cast<Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("direction")).get());
	// }

	// template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	// inline typename DefaultStepperMotorDataRetriever::Shoulders DefaultStepperMotorDataRetriever::parse_shoulders(const server::Data& data) const {
	// 	Shoulders shoulders;
	// 	using namespace server;
	// 	Data::cast<Object>(Data::cast<Object>(data).access("shoulders")).for_each(
	// 		[&shoulders](const std::string& shoulder_str, const Data& gpo_id) {
	// 			shoulders.insert({str_to_shoulder(shoulder_str), Data::cast<Integer>(gpo_id).get()});
	// 		}
	// 	);
	// 	return shoulders;
	// }

	// template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	// inline typename DefaultStepperMotorDataRetriever::States DefaultStepperMotorDataRetriever::parse_states(const server::Data& data) const {
	// 	States states;
	// 	using namespace server;
	// 	using GpoState = mcu_platform::Gpo::State;
	// 	Data::cast<Array>(Data::cast<Object>(data).access("states")).for_each(
	// 		[&states](int state_index, const Data& state_data) {
	// 			State state;
	// 			Data::cast<Object>(state_data).for_each(
	// 				[&state](const std::string& shoulder_str, const Data& gpo_state) {
	// 					state.insert({str_to_shoulder(shoulder_str), static_cast<GpoState>(Data::cast<Integer>(gpo_state).get())});
	// 				}
	// 			);
	// 			states.push_back(state);
	// 		}
	// 	);
	// 	return states;
	// }

	// inline typename DefaultStepperMotorDataRetriever::Shoulder DefaultStepperMotorDataRetriever::str_to_shoulder(const std::string& shoulder_str) {
	// 	const std::string prefix("IN");
	// 	auto prefix_pos = shoulder_str.find(prefix);
	// 	if (0 != prefix_pos) {
	// 		throw std::invalid_argument("shoulder tag doesn't start with " + prefix);
	// 	}
	// 	auto shoulder_number = std::stoi(std::string(shoulder_str.begin() + prefix.size(), shoulder_str.end()));
	// 	return static_cast<Shoulder>(shoulder_number);
	// }

	// inline typename DefaultStepperMotorDataRetriever::StepsSequence DefaultStepperMotorDataRetriever::parse_steps_sequence(const server::Data& data) const {
	// 	StepsSequence sequence;
	// 	using namespace server;
	// 	using Steps = typename StepsSequenceTask<int, int>::Steps;
	// 	Data::cast<Array>(Data::cast<Object>(data).access("sequence")).for_each(
	// 		[&sequence, this](int index, const Data& steps_data) {
	// 			sequence.push_back(
	// 				Steps(
	// 					parse_stepper_id(steps_data),
	// 					parse_steps_direction(steps_data),
	// 					parse_steps_number(steps_data),
	// 					parse_step_duration(steps_data)
	// 				)
	// 			);
	// 		}
	// 	);
	// 	return sequence;
	// }
}
#endif // DEFAULT_STEPPER_MOTOR_DATA_RETRIEVER_HPP