#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "gpio.hpp"
#include "gpo.hpp"
#include "platform.hpp"

namespace mcu_platform {
	template <typename Tgpio_id>
	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		enum class Shoulder: int { IN0, IN1, IN2, IN3 };
		using Shoulders = std::map<Shoulder, Tgpio_id>;
		using State = std::map<Shoulder, Gpio::State>;
		using States = std::vector<State>;
		
		StepperMotor(
			const Shoulders& shoulders,
			const States& states,
			const Platform<Tgpio_id> *platform
		);

		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept = default;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms);
	private:
		const States m_states;
		const Platform<Tgpio_id> *m_platform;
		std::size_t m_current_state;
		const State m_shutdown_state;
		
		using Gpos = std::map<Shoulder, std::unique_ptr<Gpo>>;
		Gpos m_gpos;

		std::size_t next_state(const Direction& direction) const;
		void init_gpos(const Shoulders& shoulders);

		using GpioState = typename Gpio::State;
	};

	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::StepperMotor(
		const Shoulders& shoulders,
		const States& states,
		const Platform<Tgpio_id> *platform
	):
		m_states(states),
		m_platform(platform),
		m_current_state(0UL),
		m_shutdown_state {{Shoulder::IN0, GpioState::LOW}, {Shoulder::IN1, GpioState::LOW}, {Shoulder::IN2, GpioState::LOW}, {Shoulder::IN3, GpioState::LOW}} {
		
		if (!m_platform) {
			throw std::invalid_argument("invalid platform ptr received");
		}
		init_gpos(shoulders);
	}

	template <typename Tgpio_id>
	inline void StepperMotor<Tgpio_id>::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) {
		while (steps_num) {
			auto next_state_index = next_state(direction);
			for (auto [shoulder, state]: m_states[next_state_index]) {
				m_gpos.at(shoulder)->set_state(state);
			}
			m_platform->delay(step_duration_ms);
			m_current_state = next_state_index;
			--steps_num;
		}
		for (auto [shoulder, state]: m_shutdown_state) {
			m_gpos.at(shoulder)->set_state(state);
		}
	}

	template <typename Tgpio_id>
	inline void StepperMotor<Tgpio_id>::init_gpos(const Shoulders& shoulders) {
		for (const auto [shoulder, gpio_id]: shoulders) {
			m_gpos.insert({shoulder, std::unique_ptr<Gpo>(dynamic_cast<Gpo *>(m_platform->create_gpio(gpio_id, Gpio::Direction::OUT)))});
		}
	}

	template <typename Tgpio_id>
	inline std::size_t StepperMotor<Tgpio_id>::next_state(const Direction& direction) const {
		auto next_cw = [this](void) {
			if (m_states.size() <= m_current_state + 1UL) {
				return 0UL;
			}
			return m_current_state + 1UL;
		};
		auto next_ccw = [this](void) {
			if (0 == m_current_state) {
				return m_states.size() - 1UL;
			}
			return m_current_state - 1UL;
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
}

#endif // STEPPER_MOTOR_HPP