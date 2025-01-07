#ifndef	STEPPER_MOTOR_REQUEST_DATA_HPP
#define	STEPPER_MOTOR_REQUEST_DATA_HPP

#include <cstddef>

#include "stepper_motor_types.hpp"

namespace manager {
	class StepperMotorRequestData {
	public:
		enum class Type: int {
			REGULAR,
			CREATE_CONFIG,
			STEPS_CONFIG
		};
		virtual ~StepperMotorRequestData() noexcept = default;
		virtual Type type() const = 0;
		virtual StepperMotorRequestData *clone() const = 0;
	};
	
	template <typename Tmotor_id>
	class StepperMotorRegularRequestData: public StepperMotorRequestData {
	public:
		StepperMotorRegularRequestData(const Tmotor_id& motor_id);
		StepperMotorRegularRequestData(const StepperMotorRegularRequestData&) = default;
		StepperMotorRegularRequestData& operator=(const StepperMotorRegularRequestData&) = default;
		
		Type type() const override;
		StepperMotorRequestData *clone() const override;

		Tmotor_id motor_id() const;
	private:
		Tmotor_id m_motor_id;
	};

	template <typename Tmotor_id>
	inline StepperMotorRegularRequestData<Tmotor_id>::StepperMotorRegularRequestData(const Tmotor_id& motor_id): m_motor_id(motor_id) {

	}

	template <typename Tmotor_id>
	inline typename StepperMotorRegularRequestData<Tmotor_id>::Type StepperMotorRegularRequestData<Tmotor_id>::type() const {
		return Type::REGULAR;
	}
	
	template <typename Tmotor_id>
	inline StepperMotorRequestData *StepperMotorRegularRequestData<Tmotor_id>::clone() const {
		return new StepperMotorRegularRequestData(*this);
	}

	template <typename Tmotor_id>
	inline Tmotor_id StepperMotorRegularRequestData<Tmotor_id>::motor_id() const {
		return m_motor_id;
	}

	template <typename Tmotor_id, typename Tcreate_cfg>
	class StepperMotorCreateRequestData: public StepperMotorRequestData {
	public:
		StepperMotorCreateRequestData(const Tmotor_id& motor_id, const Tcreate_cfg& create_cfg);
		StepperMotorCreateRequestData(const StepperMotorCreateRequestData&) = default;
		StepperMotorCreateRequestData& operator=(const StepperMotorCreateRequestData&) = default;
		
		Type type() const override;
		StepperMotorRequestData *clone() const override;

		Tmotor_id motor_id() const;
		Tcreate_cfg create_cfg() const;
	private:
		Tmotor_id m_motor_id;
		Tcreate_cfg m_create_cfg;
	};

	template <typename Tmotor_id, typename Tcreate_cfg>
	inline StepperMotorCreateRequestData<Tmotor_id, Tcreate_cfg>::StepperMotorCreateRequestData(const Tmotor_id& motor_id, const Tcreate_cfg& create_cfg): m_motor_id(motor_id), m_create_cfg(create_cfg) {

	}

	template <typename Tmotor_id, typename Tcreate_cfg>
	inline typename StepperMotorCreateRequestData<Tmotor_id, Tcreate_cfg>::Type StepperMotorCreateRequestData<Tmotor_id, Tcreate_cfg>::type() const {
		return Type::CREATE_CONFIG;
	}
	
	template <typename Tmotor_id, typename Tcreate_cfg>
	inline StepperMotorRequestData *StepperMotorCreateRequestData<Tmotor_id, Tcreate_cfg>::clone() const {
		return new StepperMotorCreateRequestData(*this);
	}

	template <typename Tmotor_id, typename Tcreate_cfg>
	inline Tmotor_id StepperMotorCreateRequestData<Tmotor_id, Tcreate_cfg>::motor_id() const {
		return m_motor_id;
	}

	template <typename Tmotor_id, typename Tcreate_cfg>
	inline Tcreate_cfg StepperMotorCreateRequestData<Tmotor_id, Tcreate_cfg>::create_cfg() const {
		return m_create_cfg;
	}

	template <typename Tmotor_id>
	class StepperMotorStepsRequestData: public StepperMotorRequestData {
	public:
		StepperMotorStepsRequestData(const Tmotor_id& motor_id, const StepperMotorDirection& direction, const std::size_t& steps_num, const std::size_t& step_duration);
		StepperMotorStepsRequestData(const StepperMotorStepsRequestData&) = default;
		StepperMotorStepsRequestData& operator=(const StepperMotorStepsRequestData&) = default;
		
		Type type() const override;
		StepperMotorRequestData *clone() const override;

		Tmotor_id motor_id() const;
		StepperMotorDirection direction() const;
		std::size_t steps_number() const;
		std::size_t step_duration() const;
	private:
		Tmotor_id m_motor_id;
		StepperMotorDirection m_direction;
		std::size_t m_steps_num;
		std::size_t m_step_duration;
	};

	template <typename Tmotor_id>
	inline StepperMotorStepsRequestData<Tmotor_id>::StepperMotorStepsRequestData(const Tmotor_id& motor_id, const StepperMotorDirection& direction, const std::size_t& steps_num, const std::size_t& step_duration): m_motor_id(motor_id), m_direction(direction), m_steps_num(steps_num), m_step_duration(step_duration) {

	}

	template <typename Tmotor_id>
	inline typename StepperMotorStepsRequestData<Tmotor_id>::Type StepperMotorStepsRequestData<Tmotor_id>::type() const {
		return Type::STEPS_CONFIG;
	}
	
	template <typename Tmotor_id>
	inline StepperMotorRequestData *StepperMotorStepsRequestData<Tmotor_id>::clone() const {
		return new StepperMotorStepsRequestData(*this);
	}

	template <typename Tmotor_id>
	inline Tmotor_id StepperMotorStepsRequestData<Tmotor_id>::motor_id() const {
		return m_motor_id;
	}

	template <typename Tmotor_id>
	inline StepperMotorDirection StepperMotorStepsRequestData<Tmotor_id>::direction() const {
		return m_direction;
	}

	template <typename Tmotor_id>
	inline std::size_t StepperMotorStepsRequestData<Tmotor_id>::steps_number() const {
		return m_steps_num;
	}

	template <typename Tmotor_id>
	inline std::size_t StepperMotorStepsRequestData<Tmotor_id>::step_duration() const {
		return m_step_duration;
	}
}

#endif // STEPPER_MOTOR_REQUEST_DATA_HPP