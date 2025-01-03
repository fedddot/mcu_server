#ifndef	STEPPER_MOTOR_REQUEST_HPP
#define	STEPPER_MOTOR_REQUEST_HPP

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>

#include "stepper_motor_types.hpp"

namespace manager {
	
	enum class StepperMotorRequestType: int {
		CREATE_STEPPER,
		DELETE_STEPPER,
		READ_STEPPER,
		STEPS
	};
	
	class StepperMotorRequest {
	public:
		virtual ~StepperMotorRequest() noexcept = default;
		virtual const StepperMotorRequestType type() const = 0;
		virtual const StepperMotorId id() const = 0;
		
		template <typename T>
		static const T& cast(const StepperMotorRequest& request);
	};

	template <typename T>
	inline const T& StepperMotorRequest::cast(const StepperMotorRequest& request) {
		try {
			return dynamic_cast<const T&>(request);
		} catch (const std::exception& e) {
			throw std::runtime_error("failed to cast stepper motor request: " + std::string(e.what()));
		}
	}

	template <typename Tcreate_cfg>
	class CreateStepperMotorRequest: public StepperMotorRequest {
	public:
		CreateStepperMotorRequest(const StepperMotorId& id, const Tcreate_cfg& create_data);
		CreateStepperMotorRequest(const CreateStepperMotorRequest&) = delete;
		CreateStepperMotorRequest& operator=(const CreateStepperMotorRequest&) = delete;

		const StepperMotorRequestType type() const override;
		const StepperMotorId id() const override;
		const Tcreate_cfg create_data() const;
	private:
		StepperMotorId m_id;
		Tcreate_cfg m_create_data;
	};
	template <typename Tcreate_cfg>
	inline CreateStepperMotorRequest<Tcreate_cfg>::CreateStepperMotorRequest(const StepperMotorId& id, const Tcreate_cfg& create_data): m_id(id), m_create_data(create_data) {

	}
	template <typename Tcreate_cfg>
	inline const StepperMotorRequestType CreateStepperMotorRequest<Tcreate_cfg>::type() const {
		return StepperMotorRequestType::CREATE_STEPPER;
	}
	template <typename Tcreate_cfg>
	inline const StepperMotorId CreateStepperMotorRequest<Tcreate_cfg>::id() const {
		return m_id;
	}
	template <typename Tcreate_cfg>
	inline const Tcreate_cfg CreateStepperMotorRequest<Tcreate_cfg>::create_data() const {
		return m_create_data;
	}

	class DeleteStepperMotorRequest: public StepperMotorRequest {
	public:
		DeleteStepperMotorRequest(const StepperMotorId& id);
		DeleteStepperMotorRequest(const DeleteStepperMotorRequest&) = delete;
		DeleteStepperMotorRequest& operator=(const DeleteStepperMotorRequest&) = delete;

		const StepperMotorRequestType type() const override;
		const StepperMotorId id() const override;
	private:
		StepperMotorId m_id;
	};
	inline DeleteStepperMotorRequest::DeleteStepperMotorRequest(const StepperMotorId& id): m_id(id) {

	}
	inline const StepperMotorRequestType DeleteStepperMotorRequest::type() const {
		return StepperMotorRequestType::DELETE_STEPPER;
	}
	inline const StepperMotorId DeleteStepperMotorRequest::id() const {
		return m_id;
	}

	class ReadStepperMotorRequest: public StepperMotorRequest {
	public:
		ReadStepperMotorRequest(const StepperMotorId& id);
		ReadStepperMotorRequest(const ReadStepperMotorRequest&) = delete;
		ReadStepperMotorRequest& operator=(const ReadStepperMotorRequest&) = delete;

		const StepperMotorRequestType type() const override;
		const StepperMotorId id() const override;
	private:
		StepperMotorId m_id;
	};
	inline ReadStepperMotorRequest::ReadStepperMotorRequest(const StepperMotorId& id): m_id(id) {

	}
	inline const StepperMotorRequestType ReadStepperMotorRequest::type() const {
		return StepperMotorRequestType::READ_STEPPER;
	}
	inline const StepperMotorId ReadStepperMotorRequest::id() const {
		return m_id;
	}

	class StepsStepperMotorRequest: public StepperMotorRequest {
	public:
		StepsStepperMotorRequest(const StepperMotorId& id, const StepperMotorDirection& direction, const std::size_t& steps_number);
		StepsStepperMotorRequest(const StepsStepperMotorRequest&) = delete;
		StepsStepperMotorRequest& operator=(const StepsStepperMotorRequest&) = delete;

		const StepperMotorRequestType type() const override;
		const StepperMotorId id() const override;
		const StepperMotorDirection direction() const;
		const std::size_t steps_number() const;
	private:
		StepperMotorId m_id;
		StepperMotorDirection m_direction;
		std::size_t m_steps_number;
	};
	inline StepsStepperMotorRequest::StepsStepperMotorRequest(const StepperMotorId& id, const StepperMotorDirection& direction, const std::size_t& steps_number): m_id(id), m_direction(direction), m_steps_number(steps_number) {

	}
	inline const StepperMotorRequestType StepsStepperMotorRequest::type() const {
		return StepperMotorRequestType::STEPS;
	}
	inline const StepperMotorId StepsStepperMotorRequest::id() const {
		return m_id;
	}
	inline const StepperMotorDirection StepsStepperMotorRequest::direction() const {
		return m_direction;
	}
	inline const std::size_t StepsStepperMotorRequest::steps_number() const {
		return m_steps_number;
	}
}

#endif // STEPPER_MOTOR_REQUEST_HPP