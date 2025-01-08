#ifndef	STEPPER_MOTOR_REQUEST_HPP
#define	STEPPER_MOTOR_REQUEST_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>

#include "stepper_motor_types.hpp"

namespace manager {
	template <typename Tcreate_cfg>
	class StepperMotorRequest {
	public:
		enum class Type: int {
			CREATE_STEPPER,
			DELETE_STEPPER,
			READ_STEPPER,
			STEPS
		};
		struct StepperMotorSteps {
			StepperMotorDirection direction;
			std::size_t steps_number;
			std::size_t step_duration;
		};

		StepperMotorRequest(const Type& type, const StepperMotorId& motor_id);
		StepperMotorRequest(const StepperMotorRequest& other);
		StepperMotorRequest& operator=(const StepperMotorRequest& other);
		virtual ~StepperMotorRequest() noexcept = default;
		
		Type type() const;
		StepperMotorId motor_id() const;
		
		void set_steps(const StepperMotorSteps& steps);
		bool has_steps() const;
		StepperMotorSteps steps() const;

		void set_create_config(const Tcreate_cfg& create_cfg);
		bool has_create_config() const;
		Tcreate_cfg create_config() const;
	private:
		Type m_type;
		StepperMotorId m_motor_id;
		std::unique_ptr<StepperMotorSteps> m_steps;
		std::unique_ptr<Tcreate_cfg> m_create_cfg;
	};

	template <typename Tcreate_cfg>	
	inline StepperMotorRequest<Tcreate_cfg>::StepperMotorRequest(const Type& type, const StepperMotorId& motor_id): m_type(type), m_motor_id(motor_id) {

	}

	template <typename Tcreate_cfg>
	inline StepperMotorRequest<Tcreate_cfg>::StepperMotorRequest(const StepperMotorRequest& other): m_type(other.m_type), m_motor_id(other.m_motor_id) {
		if (other.m_steps != nullptr) {
			m_steps = std::make_unique<StepperMotorSteps>(*(other.m_steps));
		}
		if (other.m_create_cfg != nullptr) {
			m_create_cfg = std::make_unique<Tcreate_cfg>(*(other.m_create_cfg));
		}
	}

	template <typename Tcreate_cfg>
	inline StepperMotorRequest<Tcreate_cfg>& StepperMotorRequest<Tcreate_cfg>::operator=(const StepperMotorRequest& other) {
		m_type = other.m_type;
		m_motor_id = other.m_motor_id;
		if (other.m_steps != nullptr) {
			m_steps = std::make_unique<StepperMotorSteps>(*(other.m_steps));
		}
		if (other.m_create_cfg != nullptr) {
			m_create_cfg = std::make_unique<Tcreate_cfg>(*(other.m_create_cfg));
		}
		return *this;
	}

	template <typename Tcreate_cfg>
	inline typename StepperMotorRequest<Tcreate_cfg>::Type StepperMotorRequest<Tcreate_cfg>::type() const {
		return m_type;
	}

	template <typename Tcreate_cfg>
	inline StepperMotorId StepperMotorRequest<Tcreate_cfg>::motor_id() const {
		return m_motor_id;
	}

	template <typename Tcreate_cfg>
	inline void StepperMotorRequest<Tcreate_cfg>::set_steps(const StepperMotorSteps& steps) {
		m_steps = std::make_unique<StepperMotorSteps>(steps);
	}

	template <typename Tcreate_cfg>
	inline bool StepperMotorRequest<Tcreate_cfg>::has_steps() const {
		return m_steps != nullptr;
	}

	template <typename Tcreate_cfg>
	inline typename StepperMotorRequest<Tcreate_cfg>::StepperMotorSteps StepperMotorRequest<Tcreate_cfg>::steps() const {
		if (!m_steps) {
			throw std::runtime_error("the motor steps has not been set yet");
		}
		return *m_steps;
	}

	template <typename Tcreate_cfg>
	inline void StepperMotorRequest<Tcreate_cfg>::set_create_config(const Tcreate_cfg& create_cfg) {
		m_create_cfg = std::make_unique<Tcreate_cfg>(create_cfg);
	}

	template <typename Tcreate_cfg>
	inline bool StepperMotorRequest<Tcreate_cfg>::has_create_config() const {
		return m_create_cfg != nullptr;
	}

	template <typename Tcreate_cfg>
	inline Tcreate_cfg StepperMotorRequest<Tcreate_cfg>::create_config() const {
		if (!m_create_cfg) {
			throw std::runtime_error("the motor steps has not been set yet");
		}
		return *m_create_cfg;
	}
}

#endif // STEPPER_MOTOR_REQUEST_HPP