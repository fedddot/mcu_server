#ifndef	STEPPER_MOTOR_REQUEST_DATA_HPP
#define	STEPPER_MOTOR_REQUEST_DATA_HPP

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
}

#endif // STEPPER_MOTOR_REQUEST_DATA_HPP