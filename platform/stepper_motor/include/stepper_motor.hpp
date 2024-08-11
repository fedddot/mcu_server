#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

namespace platform {
	template <typename Tgpio_id>
	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		enum class StepperMotorShoulder: int { IN0, IN1, IN2, IN3 };
		using
		StepperMotor(
			const Shoulders& shoulders,
			const MotorStates& states,
			const Executor& executor,
			const TaskIdCreator& task_id_ctor
		);

		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms);
	private:
		std::unique_ptr<Executor> m_executor;
		std::unique_ptr<TaskIdCreator> m_task_id_ctor;
		
		using GpoAllocators = std::map<Shoulder, GpoAllocator>;
		GpoAllocators m_gpo_allocators;
		static GpoAllocators init_gpo_allocators(const Shoulders& shoulders);

		using StateAllocators = std::vector<StateAllocator>;
		StateAllocators m_state_allocators;
		StateAllocators init_state_allocators(const Shoulders& shoulders, const MotorStates& states) const;
		
		std::size_t m_current_state;
		const StateAllocator m_shutdown_state_allocator;

		std::size_t next_state(const Direction& direction) const;

		void execute_sequence_task(const mcu_server::Array& sequence) const;
	};

	inline StepperMotor::StepperMotor(
		const Shoulders& shoulders,
		const MotorStates& states,
		const Executor& executor,
		const TaskIdCreator& task_id_ctor
	):
		m_executor(executor.clone()),
		m_task_id_ctor(task_id_ctor.clone()),
		m_gpo_allocators(init_gpo_allocators(shoulders)),
		m_state_allocators(init_state_allocators(shoulders, states)),
		m_shutdown_state_allocator(
			m_task_id_ctor->create(),
			shoulders,
			MotorState {{Shoulder::IN1, GpioState::LOW}, {Shoulder::IN2, GpioState::LOW}, {Shoulder::IN3, GpioState::LOW}, {Shoulder::IN4, GpioState::LOW}}
		) {
		
		using namespace mcu_server;
		Array create_tasks;
		for (const auto& [shoulder, allocator]: m_gpo_allocators) {
			create_tasks.push_back(allocator.create_data());
		}
		for (const auto& allocator: m_state_allocators) {
			create_tasks.push_back(allocator.create_data());
		}
		create_tasks.push_back(m_shutdown_state_allocator.create_data());
		execute_sequence_task(create_tasks);
	}

	inline StepperMotor::~StepperMotor() noexcept {
		using namespace mcu_server;
		Array delete_tasks;
		for (const auto& [shoulder, allocator]: m_gpo_allocators) {
			delete_tasks.push_back(allocator.delete_data());
		}
		for (const auto& allocator: m_state_allocators) {
			delete_tasks.push_back(allocator.delete_data());
		}
		delete_tasks.push_back(m_shutdown_state_allocator.delete_data());
		execute_sequence_task(delete_tasks);
	}

	inline void StepperMotor::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<int, int>::TaskType;

		Array tasks;
		DelayAllocator delay(m_task_id_ctor->create(), step_duration_ms);
		
		tasks.push_back(delay.create_data());
		
		Array steps_ids;
		while (steps_num) {
			auto next_state_index = next_state(direction);
			steps_ids.push_back(Integer(m_state_allocators[next_state_index].id()));
			steps_ids.push_back(Integer(delay.id()));
			m_current_state = next_state_index;
			--steps_num;
		}
		steps_ids.push_back(Integer(m_shutdown_state_allocator.id()));
		
		Object execute_steps_tasks;
		execute_steps_tasks.add("task_type", Integer(static_cast<int>(TaskType::EXECUTE_PERSISTENT_TASKS)));
		execute_steps_tasks.add("tasks", steps_ids);
		
		tasks.push_back(execute_steps_tasks);
		tasks.push_back(delay.delete_data());
		execute_sequence_task(tasks);
	}

	inline StepperMotor::GpoAllocators StepperMotor::init_gpo_allocators(const Shoulders& shoulders) {
		GpoAllocators allocators;
		for (const auto [shoulder, gpio_id]: shoulders) {
			allocators.insert({shoulder, GpoAllocator(gpio_id)});
		}
		return allocators;
	}

	inline StepperMotor::StateAllocators StepperMotor::init_state_allocators(const Shoulders& shoulders, const MotorStates& states) const {
		StateAllocators allocators;
		for (const auto state: states) {
			allocators.push_back(
				StateAllocator(
					m_task_id_ctor->create(),
					shoulders,
					state
				)
			);
		}
		return allocators;
	}

	inline std::size_t StepperMotor::next_state(const Direction& direction) const {
		auto next_cw = [this](void) {
			if (m_state_allocators.size() <= m_current_state + 1) {
				return 0UL;
			}
			return m_current_state + 1;
		};
		auto next_ccw = [this](void) {
			if (0 == m_current_state) {
				return m_state_allocators.size() - 1;
			}
			return m_current_state - 1;
		};
		switch (direction) {
		case Direction::CCW:
			return next_ccw();
		case Direction::CW:
			return next_cw();
		default:
			throw std::invalid_argument("unsupported direction");
		}
	}

	inline void StepperMotor::execute_sequence_task(const mcu_server::Array& sequence) const {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<int, int>::TaskType;
		Object execute_tasks;
		execute_tasks.add("task_type", Integer(static_cast<int>(TaskType::SEQUENCE)));
		execute_tasks.add("tasks", sequence);
		m_executor->execute(execute_tasks);
	}

}

#endif // STEPPER_MOTOR_HPP