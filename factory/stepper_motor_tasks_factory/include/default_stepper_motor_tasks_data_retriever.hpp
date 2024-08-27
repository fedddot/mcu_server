#ifndef	DEFAULT_STEPPER_MOTOR_DATA_RETRIEVER_HPP
#define	DEFAULT_STEPPER_MOTOR_DATA_RETRIEVER_HPP

#include "array.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_tasks_data_retriever.hpp"
#include "stepper_motor_tasks_factory.hpp"
#include "string.hpp"

namespace mcu_factory {
	template <typename Tgpio_id, typename Tstepper_id>
	class DefaultStepperMotorDataRetriever: public StepperMotorTasksDataRetriever<typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::TaskType, Tgpio_id, Tstepper_id, typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::Steps, typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepsSequence> {
	private:
		using ParentRetriever = StepperMotorTasksDataRetriever<typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::TaskType, Tgpio_id, Tstepper_id, typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::Steps, typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepsSequence>; 
	public:
		using Direction = typename ParentRetriever::Direction;
		using Shoulders = typename ParentRetriever::Shoulders;
		using States = typename ParentRetriever::States;
		using TaskType = typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::TaskType;
		using Steps = typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::Steps;
		using StepsSequence = typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepsSequence;
		
		DefaultStepperMotorDataRetriever() = default;
		DefaultStepperMotorDataRetriever(const DefaultStepperMotorDataRetriever& other) = default;
		DefaultStepperMotorDataRetriever& operator=(const DefaultStepperMotorDataRetriever& other) = delete;

		ParentRetriever *clone() const override;

		bool is_stepper_motor_task_creatable(const server::Data& data) const override;
		TaskType retrieve_task_type(const server::Data& data) const override;
		Tgpio_id retrieve_gpio_id(const server::Data& data) const override;
		Tstepper_id retrieve_stepper_id(const server::Data& data) const override;
		Direction retrieve_dir(const server::Data& data) const override;
		unsigned int retrieve_step_duration(const server::Data& data) const override;
		unsigned int retrieve_steps_number(const server::Data& data) const override;
		Shoulders retrieve_shoulders(const server::Data& data) const override;
		States retrieve_states(const server::Data& data) const override;
		Steps retrieve_steps(const server::Data& data) const override;
		StepsSequence retrieve_steps_sequence(const server::Data& data) const override;
	private:
		using Shoulder = typename mcu_platform::StepperMotor<int>::Shoulder;
		using State = typename mcu_platform::StepperMotor<int>::State;
		static Shoulder str_to_shoulder(const std::string& shoulder_str);
	};

	template <typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::ParentRetriever *DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::clone() const {
		return new DefaultStepperMotorDataRetriever(*this);
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline bool DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::is_stepper_motor_task_creatable(const server::Data& data) const {
		using namespace server;
		return "stepper_motor" == Data::cast<String>(Data::cast<Object>(data).access("domain")).get();
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::TaskType DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_task_type(const server::Data& data) const {
		using namespace server;
		return static_cast<TaskType>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline Tstepper_id DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_stepper_id(const server::Data& data) const {
		using namespace server;
		return static_cast<Tstepper_id>(Data::cast<Integer>(Data::cast<Object>(data).access("stepper_id")).get());
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline unsigned int DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_step_duration(const server::Data& data) const {
		using namespace server;
		return static_cast<unsigned int>(Data::cast<Integer>(Data::cast<Object>(data).access("step_duration_ms")).get());
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline unsigned int DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_steps_number(const server::Data& data) const {
		using namespace server;
		return static_cast<unsigned int>(Data::cast<Integer>(Data::cast<Object>(data).access("steps_number")).get());
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::Direction DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_dir(const server::Data& data) const {
		using namespace server;
		return static_cast<Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("direction")).get());
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::Shoulders DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_shoulders(const server::Data& data) const {
		Shoulders shoulders;
		using namespace server;
		Data::cast<Object>(Data::cast<Object>(data).access("shoulders")).for_each(
			[&shoulders](const std::string& shoulder_str, const Data& gpo_id) {
				shoulders.insert({str_to_shoulder(shoulder_str), Data::cast<Integer>(gpo_id).get()});
			}
		);
		return shoulders;
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::States DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_states(const server::Data& data) const {
		States states;
		using namespace server;
		using GpoState = mcu_platform::Gpo::State;
		Data::cast<Array>(Data::cast<Object>(data).access("states")).for_each(
			[&states](int state_index, const Data& state_data) {
				State state;
				Data::cast<Object>(state_data).for_each(
					[&state](const std::string& shoulder_str, const Data& gpo_state) {
						state.insert({str_to_shoulder(shoulder_str), static_cast<GpoState>(Data::cast<Integer>(gpo_state).get())});
					}
				);
				states.push_back(state);
			}
		);
		return states;
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::Shoulder DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::str_to_shoulder(const std::string& shoulder_str) {
		const std::string prefix("IN");
		auto prefix_pos = shoulder_str.find(prefix);
		if (0 != prefix_pos) {
			throw std::invalid_argument("shoulder tag doesn't start with " + prefix);
		}
		auto shoulder_number = std::stoi(std::string(shoulder_str.begin() + prefix.size(), shoulder_str.end()));
		return static_cast<Shoulder>(shoulder_number);
	}

	template <typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::Steps DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_steps(const server::Data& data) const {
		return Steps {
			.stepper_id = retrieve_stepper_id(data),
			.direction = retrieve_dir(data),
			.steps_number = retrieve_steps_number(data),
			.step_duration_ms = retrieve_step_duration(data)
		};
	}
	
	template <typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::StepsSequence DefaultStepperMotorDataRetriever<Tgpio_id, Tstepper_id>::retrieve_steps_sequence(const server::Data& data) const {
		StepsSequence sequence;
		using namespace server;
		Data::cast<Array>(Data::cast<Object>(data).access("sequence")).for_each(
			[&sequence, this](int index, const Data& steps_data) {
				sequence.push_back(retrieve_steps(steps_data));
			}
		);
		return sequence;
	}
}
#endif // DEFAULT_STEPPER_MOTOR_DATA_RETRIEVER_HPP