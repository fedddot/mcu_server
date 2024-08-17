#ifndef	STEPS_SEQUENCE_TASK_HPP
#define	STEPS_SEQUENCE_TASK_HPP

#include <memory>
#include <vector>

#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "stepper_motor.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tstepper_id, typename Tgpio_id>
	class StepsSequenceTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using StepperMotorInventory = mcu_platform::Inventory<Tstepper_id, mcu_platform::StepperMotor<Tgpio_id>>;
		class Steps {
		public:
			using Direction = typename mcu_platform::StepperMotor<Tgpio_id>::Direction;
			Steps(
				const Tstepper_id& stepper_id,
				const Direction& direction,
				const unsigned int steps_number,
				const unsigned int step_duration_ms
			);
			Steps(const Steps& other) = default;
			Steps& operator=(const Steps& other) = default;
			virtual ~Steps() noexcept = default;

			Tstepper_id stepper_id() const;
			Direction direction() const;
			unsigned int steps_number() const;
			unsigned int step_duration_ms() const;
		private:
			Tstepper_id m_stepper_id;
			Direction m_direction;
			unsigned int m_steps_number;
			unsigned int m_step_duration_ms;
		};
		using StepsSequence = std::vector<Steps>;
		using ReportCreator = typename mcu_server::Creator<mcu_server::Data *(int)>;
		
		StepsSequenceTask(
			StepperMotorInventory *inventory,
			const StepsSequence& steps_sequence,
			const ReportCreator& report_ctor
		);
		StepsSequenceTask(const StepsSequenceTask& other) = delete;
		StepsSequenceTask& operator=(const StepsSequenceTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		StepperMotorInventory * const m_inventory;
		const StepsSequence m_steps_sequence;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepsSequenceTask<Tstepper_id, Tgpio_id>::StepsSequenceTask(
		StepperMotorInventory *inventory,
		const StepsSequence& steps_sequence,
		const ReportCreator& report_ctor
	): m_inventory(inventory), m_steps_sequence(steps_sequence), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline mcu_server::Data *StepsSequenceTask<Tstepper_id, Tgpio_id>::execute() const {
		for (const auto& steps: m_steps_sequence) {
			m_inventory->access(steps.stepper_id())->steps(steps.direction(), steps.steps_number(), steps.step_duration_ms());
		}
		return m_report_ctor->create(0);
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepsSequenceTask<Tstepper_id, Tgpio_id>::Steps::Steps(
		const Tstepper_id& stepper_id,
		const Direction& direction,
		const unsigned int steps_number,
		const unsigned int step_duration_ms
	): m_stepper_id(stepper_id), m_direction(direction), m_steps_number(steps_number), m_step_duration_ms(step_duration_ms) {

	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline Tstepper_id StepsSequenceTask<Tstepper_id, Tgpio_id>::Steps::stepper_id() const {
		return m_stepper_id;
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline typename StepsSequenceTask<Tstepper_id, Tgpio_id>::Steps::Direction StepsSequenceTask<Tstepper_id, Tgpio_id>::Steps::direction() const {
		return m_direction;
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline unsigned int StepsSequenceTask<Tstepper_id, Tgpio_id>::Steps::steps_number() const {
		return m_steps_number;
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline unsigned int StepsSequenceTask<Tstepper_id, Tgpio_id>::Steps::step_duration_ms() const {
		return m_step_duration_ms;
	}
}
#endif // STEPS_SEQUENCE_TASK_HPP