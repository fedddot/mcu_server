#ifndef	STEPPER_IPC_DATA_INFRA_HPP
#define	STEPPER_IPC_DATA_INFRA_HPP

#include <vector>

#include "json/value.h"

#include "stepper_motor.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"

namespace ipc {
	using RawData = std::vector<char>;
	
	inline Json::Value stepper_response_to_json_value(const manager::StepperMotorResponse& response) {
		Json::Value json_data;
		json_data["result"] = static_cast<int>(response.code());
		if (response.state()) {
			json_data["state"] = static_cast<int>(response.state().value());
		}
		if (response.message()) {
			json_data["message"] = response.message().value();
		}
		return json_data;
	}

	inline manager::StepperMotorRequest json_value_to_stepper_request(const Json::Value& json_request) {
		return manager::StepperMotorRequest {
			.direction = static_cast<manager::StepperMotor::Direction>(json_request["direction"].asInt()),
			.steps_number = json_request["steps_number"].asUInt64(),
			.step_duration_ms = json_request["step_duration_ms"].asUInt64()
		};
	}
}

#endif // STEPPER_IPC_DATA_INFRA_HPP