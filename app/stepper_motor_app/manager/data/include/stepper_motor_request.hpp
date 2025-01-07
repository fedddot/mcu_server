#ifndef	STEPPER_MOTOR_REQUEST_HPP
#define	STEPPER_MOTOR_REQUEST_HPP

#include <memory>
#include <stdexcept>

#include "stepper_motor_request_data.hpp"

namespace manager {

	class StepperMotorRequest {
	public:
		enum class Type: int {
			CREATE_STEPPER,
			DELETE_STEPPER,
			READ_STEPPER,
			STEPS
		};
		StepperMotorRequest(const Type& type);
		StepperMotorRequest(const StepperMotorRequest& other);
		StepperMotorRequest& operator=(const StepperMotorRequest& other);
		virtual ~StepperMotorRequest() noexcept = default;
		
		Type type() const;
		void set_data(const StepperMotorRequestData& data);
		bool has_data() const;
		const StepperMotorRequestData& data() const;
	private:
		Type m_type;
		std::unique_ptr<StepperMotorRequestData> m_data;
	};
	
	inline StepperMotorRequest::StepperMotorRequest(const Type& type): m_type(type) {

	}

	inline StepperMotorRequest::StepperMotorRequest(const StepperMotorRequest& other): m_type(other.m_type), m_data(other.m_data->clone()) {

	}

	inline StepperMotorRequest& StepperMotorRequest::operator=(const StepperMotorRequest& other) {
		m_type = other.m_type;
		m_data = std::unique_ptr<StepperMotorRequestData>(other.m_data->clone());
		return *this;
	}

	inline typename StepperMotorRequest::Type StepperMotorRequest::type() const {
		return m_type;
	}

	inline void StepperMotorRequest::set_data(const StepperMotorRequestData& data) {
		m_data = std::unique_ptr<StepperMotorRequestData>(data.clone());
	}

	inline bool StepperMotorRequest::has_data() const {
		return m_data != nullptr;
	}

	inline const StepperMotorRequestData& StepperMotorRequest::data() const {
		if (m_data == nullptr) {
			throw std::runtime_error("data has not been set yet");
		}
		return *m_data;
	}
}

#endif // STEPPER_MOTOR_REQUEST_HPP