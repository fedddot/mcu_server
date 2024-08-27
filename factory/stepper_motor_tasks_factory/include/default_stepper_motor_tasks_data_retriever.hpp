#ifndef	DEFAULT_STEPPER_MOTOR_DATA_RETRIEVER_HPP
#define	DEFAULT_STEPPER_MOTOR_DATA_RETRIEVER_HPP

#include "array.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_tasks_data_retriever.hpp"
#include "string.hpp"
#include <vector>

namespace mcu_factory {
	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	class DefaultStepperMotorDataRetriever: public StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id> {
	public:
		using Direction = typename StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::Direction;
		using Shoulders = typename StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::Shoulders;
		using States = typename StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::States;
		struct Steps {
			Tstepper_id stepper_id;
			Direction direction;
			unsigned int steps_number;
			unsigned int step_duration_ms;
		};
		using StepsSequence = std::vector<Steps>;
		
		DefaultStepperMotorDataRetriever() = default;
		DefaultStepperMotorDataRetriever(const DefaultStepperMotorDataRetriever& other) = default;
		DefaultStepperMotorDataRetriever& operator=(const DefaultStepperMotorDataRetriever& other) = delete;

		StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id> *clone() const override;

		bool is_stepper_motor_task_creatable(const server::Data& data) const override;
		Ttask_type retrieve_task_type(const server::Data& data) const override;
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

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline StepperMotorTasksDataRetriever<Ttask_type, Tgpio_id, Tstepper_id> *DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::clone() const {
		return new DefaultStepperMotorDataRetriever(*this);
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline bool DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::is_stepper_motor_task_creatable(const server::Data& data) const {
		using namespace server;
		return "stepper_motor" == Data::cast<String>(Data::cast<Object>(data).access("domain")).get();
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline Ttask_type DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_task_type(const server::Data& data) const {
		using namespace server;
		return static_cast<Ttask_type>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline Tstepper_id DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_stepper_id(const server::Data& data) const {
		using namespace server;
		return static_cast<Tstepper_id>(Data::cast<Integer>(Data::cast<Object>(data).access("stepper_id")).get());
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline unsigned int DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_step_duration(const server::Data& data) const {
		using namespace server;
		return static_cast<unsigned int>(Data::cast<Integer>(Data::cast<Object>(data).access("step_duration_ms")).get());
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline unsigned int DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_steps_number(const server::Data& data) const {
		using namespace server;
		return static_cast<unsigned int>(Data::cast<Integer>(Data::cast<Object>(data).access("steps_number")).get());
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::Direction DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_dir(const server::Data& data) const {
		using namespace server;
		return static_cast<Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("direction")).get());
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::Shoulders DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_shoulders(const server::Data& data) const {
		Shoulders shoulders;
		using namespace server;
		Data::cast<Object>(Data::cast<Object>(data).access("shoulders")).for_each(
			[&shoulders](const std::string& shoulder_str, const Data& gpo_id) {
				shoulders.insert({str_to_shoulder(shoulder_str), Data::cast<Integer>(gpo_id).get()});
			}
		);
		return shoulders;
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::States DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_states(const server::Data& data) const {
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

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::Shoulder DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::str_to_shoulder(const std::string& shoulder_str) {
		const std::string prefix("IN");
		auto prefix_pos = shoulder_str.find(prefix);
		if (0 != prefix_pos) {
			throw std::invalid_argument("shoulder tag doesn't start with " + prefix);
		}
		auto shoulder_number = std::stoi(std::string(shoulder_str.begin() + prefix.size(), shoulder_str.end()));
		return static_cast<Shoulder>(shoulder_number);
	}

	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::Steps DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_steps(const server::Data& data) const {
		return Steps {
			.stepper_id = retrieve_stepper_id(data),
			.direction = retrieve_dir(data),
			.steps_number = retrieve_steps_number(data),
			.step_duration_ms = retrieve_step_duration(data)
		};
	}
	
	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id>
	inline typename DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::StepsSequence DefaultStepperMotorDataRetriever<Ttask_type, Tgpio_id, Tstepper_id>::retrieve_steps_sequence(const server::Data& data) const {
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