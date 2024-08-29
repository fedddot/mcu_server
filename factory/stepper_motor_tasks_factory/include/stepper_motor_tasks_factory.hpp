#ifndef	STEPPER_MOTOR_TASKS_FACTORY_HPP
#define	STEPPER_MOTOR_TASKS_FACTORY_HPP

#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "custom_task.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_tasks_data_retriever.hpp"
#include "task.hpp"
#include "task_creator.hpp"

namespace mcu_factory {
	template <typename Tstepper_id, typename Tgpio_id>
	class StepperMotorTasksFactory: public TaskCreator {
	public:
		enum class TaskType: int {
			CREATE_STEPPER_MOTOR,
			DELETE_STEPPER_MOTOR,
			STEPS,
			STEPS_SEQUENCE
		};

		using FactoryPlatform = mcu_platform::Platform<Tgpio_id>;
		using ResultReporter = server::Creator<server::Data *(int)>;
		using MotorTask = server::Task<server::Data *(void)>;
		using MotorInventory = mcu_platform::Inventory<Tstepper_id, mcu_platform::StepperMotor<Tgpio_id>>;
		using Direction = typename mcu_platform::StepperMotor<Tgpio_id>::Direction;
		
		struct Steps {
			Tstepper_id stepper_id;
			Direction direction;
			unsigned int steps_number;
			unsigned int step_duration_ms;
		};
		using StepsSequence = std::vector<Steps>;

		using DataRetriever = StepperMotorTasksDataRetriever<TaskType, Tgpio_id, Tstepper_id, Steps, StepsSequence>;


		StepperMotorTasksFactory(
			MotorInventory *inventory,
			const FactoryPlatform *platform,
			const DataRetriever& retriever,
			const ResultReporter& result_reporter
		);
		StepperMotorTasksFactory(const StepperMotorTasksFactory& other);
		StepperMotorTasksFactory& operator=(const StepperMotorTasksFactory& other) = delete;

		MotorTask *create(const server::Data& data) const override;
		bool is_creatable(const server::Data& data) const override;
		server::Creator<MotorTask *(const server::Data&)> *clone() const override;
		TaskCreator *clone_task_creator() const override;
	private:
		mutable MotorInventory *m_inventory;
		const FactoryPlatform *m_platform;
		const std::unique_ptr<DataRetriever> m_retriever;
		const std::unique_ptr<ResultReporter> m_result_reporter;
		
		using Shoulders = typename mcu_platform::StepperMotor<Tgpio_id>::Shoulders;
		using States = typename mcu_platform::StepperMotor<Tgpio_id>::States;

		MotorTask *create_task(const Tstepper_id& id, const Shoulders& shoulders, const States& states) const;
		MotorTask *delete_task(const Tstepper_id& id) const;
		MotorTask *steps_task(const Steps& steps) const;
		MotorTask *steps_sequence_task(const StepsSequence& steps_sequence) const;
	};

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepperMotorTasksFactory(
		MotorInventory *inventory,
		const FactoryPlatform *platform,
		const DataRetriever& retriever,
		const ResultReporter& result_reporter
	):
		m_inventory(inventory),
		m_platform(platform),
		m_retriever(retriever.clone()),
		m_result_reporter(result_reporter.clone()) {
		
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory pointer received");
		}
		if (!m_platform) {
			throw std::invalid_argument("invalid platform pointer received");
		}
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepperMotorTasksFactory(const StepperMotorTasksFactory& other):
		m_inventory(other.m_inventory),
		m_platform(other.m_platform),
		m_retriever(other.m_retriever->clone()),
		m_result_reporter(other.m_result_reporter->clone()) {
		
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::MotorTask *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::create(const server::Data& data) const {
		switch (m_retriever->retrieve_task_type(data)) {
		case TaskType::CREATE_STEPPER_MOTOR:
			return create_task(
				m_retriever->retrieve_stepper_id(data),
				m_retriever->retrieve_shoulders(data),
				m_retriever->retrieve_states(data)
			);
		case TaskType::DELETE_STEPPER_MOTOR:
			return delete_task(m_retriever->retrieve_stepper_id(data));
		case TaskType::STEPS:
			return steps_task(m_retriever->retrieve_steps(data));
		case TaskType::STEPS_SEQUENCE:
			return steps_sequence_task(m_retriever->retrieve_steps_sequence(data));
		default:
			throw std::invalid_argument("unsupported task type received");
		}
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline bool StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::is_creatable(const server::Data& data) const {
		return m_retriever->is_stepper_motor_task_creatable(data);
	}
	
	template <typename Tstepper_id, typename Tgpio_id>
	inline server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::clone() const {
		return clone_task_creator();
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline TaskCreator *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::clone_task_creator() const {
		return new StepperMotorTasksFactory(*this);
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::MotorTask *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::create_task(const Tstepper_id& id, const Shoulders& shoulders, const States& states) const {
		using namespace server;
		using namespace server_utl;
		using namespace mcu_platform;
		auto inventory(m_inventory);
		auto platform(m_platform);
		std::shared_ptr<ResultReporter> reporter(m_result_reporter->clone());
		return new CustomTask<Data *(void)>(
			[id, shoulders, states, inventory, platform, reporter](void)-> Data * {
				auto motor_ptr(new StepperMotor<Tgpio_id>(shoulders, states, platform));
				try {
					inventory->put(id, motor_ptr);
					return reporter->create(0);
				} catch (...) {
					delete motor_ptr;
					throw;
				}
			}
		);
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::MotorTask *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::delete_task(const Tstepper_id& id) const {
		using namespace server;
		using namespace server_utl;
		using namespace mcu_platform;
		auto inventory(m_inventory);
		std::shared_ptr<ResultReporter> reporter(m_result_reporter->clone());
		return new CustomTask<Data *(void)>(
			[id, inventory, reporter](void)-> Data * {
				delete inventory->pull(id);
				return reporter->create(0);
			}
		);
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::MotorTask *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::steps_task(const Steps& steps) const {
		using namespace server;
		using namespace server_utl;
		using namespace mcu_platform;
		auto inventory(m_inventory);
		std::shared_ptr<ResultReporter> reporter(m_result_reporter->clone());
		return new CustomTask<Data *(void)>(
			[steps, inventory, reporter](void)-> Data * {
				inventory->access(steps.stepper_id)->steps(steps.direction, steps.steps_number, steps.step_duration_ms);
				return reporter->create(0);
			}
		);
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline typename StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::MotorTask *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::steps_sequence_task(const StepsSequence& steps_sequence) const {
		using namespace server;
		using namespace server_utl;
		using namespace mcu_platform;
		auto inventory(m_inventory);
		std::shared_ptr<ResultReporter> reporter(m_result_reporter->clone());
		return new CustomTask<Data *(void)>(
			[steps_sequence, inventory, reporter](void)-> Data * {
				for (const auto& steps: steps_sequence) {
					inventory->access(steps.stepper_id)->steps(steps.direction, steps.steps_number, steps.step_duration_ms);
				}
				return reporter->create(0);
			}
		);
	}
}
#endif // STEPPER_MOTOR_TASKS_FACTORY_HPP