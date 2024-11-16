#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <map>
#include <stdexcept>
#include <vector>

#include "gpo.hpp"

namespace manager {
	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		enum class ControlOutput: int {
			ENA,	// enable A half-bridge
			ENB		// enable B half-bridge
		};
		using ControlOutputs = std::map<ControlOutput, Gpo *>;

		enum class DirectionOutput: int {
			A_BTM,	// A half-bridge bottom shoulder
			A_TOP,	// A half-bridge top shoulder
			B_BTM,	// B half-bridge bottom shoulder
			B_TOP,	// B half-bridge top shoulder
		};
		using DirectionOutputs = std::map<DirectionOutput, Gpo *>;
		using MotorState = std::map<DirectionOutput, Gpo::State>;
		using MotorStates = std::vector<MotorState>;
		
		StepperMotor(const ControlOutputs& control_outputs, const DirectionOutputs& direction_outputs, const MotorStates& states);
		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;

		void enable();
		void disable();
		bool enabled() const;
		void step(const Direction& direction);
	private:
		const ControlOutputs m_control_outputs;
		const DirectionOutputs m_direction_outputs;
		const MotorStates m_states;
		std::size_t m_current_state_number;

		std::size_t get_next_state(const Direction& direction) const;
		void apply_state(const MotorState& state);

		template <typename Tkey>
		static void validate_outputs(const std::map<Tkey, Gpo *>& outputs, const std::vector<Tkey>& required_keys);
	};

	
	inline StepperMotor::StepperMotor(const ControlOutputs& control_outputs, const DirectionOutputs& direction_outputs, const MotorStates& states): m_control_outputs(control_outputs), m_direction_outputs(direction_outputs), m_states(states), m_current_state_number(0UL) {
		validate_outputs(m_control_outputs, {ControlOutput::ENA, ControlOutput::ENB});
		validate_outputs(m_direction_outputs, {DirectionOutput::A_TOP, DirectionOutput::A_BTM, DirectionOutput::B_TOP, DirectionOutput::B_BTM});
		if (2UL < m_states.size()) {
			throw std::invalid_argument("states vector is too short");
		}
		disable();
		apply_state(m_states[0]);
	}
	
	inline void StepperMotor::enable() {
		using GpoState = typename Gpo::State;
		for (const auto& output: {ControlOutput::ENA, ControlOutput::ENB}) {
			m_control_outputs.at(output)->set_state(GpoState::HIGH);
		}
	}

	inline void StepperMotor::disable() {
		using GpoState = typename Gpo::State;
		for (const auto& output: {ControlOutput::ENA, ControlOutput::ENB}) {
			m_control_outputs.at(output)->set_state(GpoState::LOW);
		}
	}

	inline bool StepperMotor::enabled() const {
		using GpoState = typename Gpo::State;
		for (const auto& output: {ControlOutput::ENA, ControlOutput::ENB}) {
			if (GpoState::LOW == m_control_outputs.at(output)->state()) {
				return false;
			}
		}
		return true;
	}

	inline void StepperMotor::step(const Direction& direction) {
		auto next_state = get_next_state(direction);
		apply_state(m_states[next_state]);
		m_current_state_number = next_state;
	}

	
	inline std::size_t StepperMotor::get_next_state(const Direction& direction) const {
		auto get_next_cw = [this](const std::size_t& from) {
			auto next(from + 1);
			if (m_states.size() <= next) {
				return std::size_t(0UL);
			}
			return next;
		};
		auto get_next_ccw = [this](const std::size_t& from) {
			if (0UL == from) {
				return m_states.size() - 1UL;
			}
			return from - 1UL;
		};
		switch (direction) {
		case Direction::CW:
			return get_next_cw(m_current_state_number);
		case Direction::CCW:
			return get_next_ccw(m_current_state_number);
		default:
			throw std::invalid_argument("invalid direction received");
		}
	}

	
	inline void StepperMotor::apply_state(const MotorState& state) {
		for (const auto& [output, gpo_state]: state) {
			m_direction_outputs.at(output)->set_state(gpo_state);
		}
	}

	template <typename Tkey>
	inline void StepperMotor::validate_outputs(const std::map<Tkey, Gpo *>& outputs, const std::vector<Tkey>& required_keys) {
		for (const auto& key: required_keys) {
			const auto iter = outputs.find(key);
			if (outputs.end() == iter) {
				throw std::invalid_argument("missing required control output");
			}
			if (nullptr == iter->second) {
				throw std::invalid_argument("gpo pointer is nullptr");
			}
		}
	}
}

#endif // STEPPER_MOTOR_HPP