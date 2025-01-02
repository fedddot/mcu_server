#ifndef	STEPPER_MOTOR_DATA_HPP
#define	STEPPER_MOTOR_DATA_HPP

#include <cstddef>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

#include "data.hpp"

namespace manager {
	using StepperMotorId = std::string;

	enum class StepperMotorDirection: int {
		CW,
		CCW
	};

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

	class CreateStepperMotorRequest: public StepperMotorRequest {
	public:
		CreateStepperMotorRequest(const StepperMotorId& id, const Data& create_data);
		CreateStepperMotorRequest(const CreateStepperMotorRequest&) = delete;
		CreateStepperMotorRequest& operator=(const CreateStepperMotorRequest&) = delete;

		const StepperMotorRequestType type() const override;
		const StepperMotorId id() const override;
		const Data& create_data() const;
	private:
		StepperMotorId m_id;
		std::unique_ptr<Data> m_create_data;
	};

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
}

#endif // STEPPER_MOTOR_DATA_HPP