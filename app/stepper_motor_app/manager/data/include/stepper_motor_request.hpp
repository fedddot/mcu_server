#ifndef	STEPPER_MOTOR_REQUEST_HPP
#define	STEPPER_MOTOR_REQUEST_HPP

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
		StepperMotorRequest(const Type& type, const StepperMotorId& motor_id);
		StepperMotorRequest(const StepperMotorRequest& other);
		StepperMotorRequest& operator=(const StepperMotorRequest& other);
		virtual ~StepperMotorRequest() noexcept = default;
		
		Type type() const;
		StepperMotorId motor_id() const;
		
		void set_state(const StepperMotorState& state);
		bool has_state() const;
		StepperMotorState state() const;

		void set_create_config(const Tcreate_cfg& create_cfg);
		bool has_create_config() const;
		Tcreate_cfg create_config() const;
	private:
		Type m_type;
		StepperMotorId m_motor_id;
		std::unique_ptr<StepperMotorState> m_state;
		std::unique_ptr<Tcreate_cfg> m_create_cfg;
	};

	template <typename Tcreate_cfg>	
	inline StepperMotorRequest<Tcreate_cfg>::StepperMotorRequest(const Type& type, const StepperMotorId& motor_id): m_type(type), m_motor_id(motor_id) {

	}

	template <typename Tcreate_cfg>
	inline StepperMotorRequest<Tcreate_cfg>::StepperMotorRequest(const StepperMotorRequest& other): m_type(other.m_type), m_motor_id(other.m_motor_id) {
		if (other.m_state != nullptr) {
			m_state = std::make_unique<StepperMotorState>(*(other.m_state));
		}
		if (other.m_create_cfg != nullptr) {
			m_create_cfg = std::make_unique<Tcreate_cfg>(*(other.m_create_cfg));
		}
	}

	template <typename Tcreate_cfg>
	inline StepperMotorRequest<Tcreate_cfg>& StepperMotorRequest<Tcreate_cfg>::operator=(const StepperMotorRequest& other) {
		m_type = other.m_type;
		m_motor_id = other.m_motor_id;
		if (other.m_state != nullptr) {
			m_state = std::make_unique<StepperMotorState>(*(other.m_state));
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
	inline void StepperMotorRequest<Tcreate_cfg>::set_state(const StepperMotorState& state) {
		m_state = std::make_unique<StepperMotorState>(state);
	}

	template <typename Tcreate_cfg>
	inline bool StepperMotorRequest<Tcreate_cfg>::has_state() const {
		return m_state != nullptr;
	}

	template <typename Tcreate_cfg>
	inline StepperMotorState StepperMotorRequest<Tcreate_cfg>::state() const {
		if (!m_state) {
			throw std::runtime_error("the motor state has not been set yet");
		}
		return *m_state;
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
			throw std::runtime_error("the motor state has not been set yet");
		}
		return *m_create_cfg;
	}
}

#endif // STEPPER_MOTOR_REQUEST_HPP