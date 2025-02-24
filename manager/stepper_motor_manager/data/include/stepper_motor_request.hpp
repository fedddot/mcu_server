#ifndef	STEPPER_MOTOR_REQUEST_HPP
#define	STEPPER_MOTOR_REQUEST_HPP

#include <cstddef>
#include <optional>

#include "stepper_motor.hpp"

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
		struct Steps {
			StepperMotor::Direction direction;
			std::size_t steps_number;
			std::size_t step_duration;
		};

		StepperMotorRequest(const Type& type, const std::string& motor_id);
		StepperMotorRequest(const StepperMotorRequest& other) = default;
		StepperMotorRequest& operator=(const StepperMotorRequest& other) = default;
		virtual ~StepperMotorRequest() noexcept = default;
		
		Type type() const;
		std::string motor_id() const;
		
		void set_steps(const Steps& steps);
		std::optional<Steps> steps() const;

		void set_create_config(const Tcreate_cfg& create_cfg);
		std::optional<Tcreate_cfg> create_config() const;
	private:
		Type m_type;
		std::string m_motor_id;
		std::optional<Steps> m_steps;
		std::optional<Tcreate_cfg> m_create_cfg;
	};

	template <typename Tcreate_cfg>	
	inline StepperMotorRequest<Tcreate_cfg>::StepperMotorRequest(const Type& type, const std::string& motor_id): m_type(type), m_motor_id(motor_id) {

	}

	template <typename Tcreate_cfg>
	inline typename StepperMotorRequest<Tcreate_cfg>::Type StepperMotorRequest<Tcreate_cfg>::type() const {
		return m_type;
	}

	template <typename Tcreate_cfg>
	inline std::string StepperMotorRequest<Tcreate_cfg>::motor_id() const {
		return m_motor_id;
	}

	template <typename Tcreate_cfg>
	inline void StepperMotorRequest<Tcreate_cfg>::set_steps(const Steps& steps) {
		m_steps = steps;
	}

	template <typename Tcreate_cfg>
	inline std::optional<typename StepperMotorRequest<Tcreate_cfg>::Steps> StepperMotorRequest<Tcreate_cfg>::steps() const {
		return m_steps;
	}

	template <typename Tcreate_cfg>
	inline void StepperMotorRequest<Tcreate_cfg>::set_create_config(const Tcreate_cfg& create_cfg) {
		m_create_cfg = create_cfg;
	}

	template <typename Tcreate_cfg>
	inline std::optional<Tcreate_cfg> StepperMotorRequest<Tcreate_cfg>::create_config() const {
		return m_create_cfg;
	}
}

#endif // STEPPER_MOTOR_REQUEST_HPP