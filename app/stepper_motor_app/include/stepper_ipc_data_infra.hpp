#ifndef	STEPPER_IPC_DATA_INFRA_HPP
#define	STEPPER_IPC_DATA_INFRA_HPP

#include <cstddef>
#include <vector>

#include "json/value.h"

#include "stepper_motor_data.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"

namespace ipc {
	using RawData = std::vector<char>;
	
	inline Json::Value stepper_response_to_json_value(const manager::StepperMotorResponse& response) {
		Json::Value json_data;
		json_data["result"] = static_cast<int>(response.code);
		if (response.state) {
			json_data["state"] = static_cast<int>(response.state.value());
		}
		if (response.message) {
			json_data["message"] = response.message.value();
		}
		return json_data;
	}

	inline manager::StepperMotorResponse json_value_to_stepper_response(const Json::Value& json_response) {
		manager::StepperMotorResponse response;
		if (!json_response.isMember("result")) {
			throw std::runtime_error("Invalid JSON response: missing 'result' field");
		}
		response.code = static_cast<manager::StepperMotorResponse::ResultCode>(json_response["result"].asInt());

		if (json_response.isMember("state")) {
			response.state = static_cast<manager::State>(json_response["state"].asInt());
		}
		if (json_response.isMember("message")) {
			response.message = json_response["message"].asString();
		}
		return response;
	}

	inline Json::Value stepper_request_to_json_value(const manager::StepperMotorRequest& request) {
		Json::Value json_data;
		json_data["direction"] = Json::Int(static_cast<int>(request.direction));
		json_data["steps_number"] = Json::UInt64(request.steps_number);
		json_data["step_duration_ms"] = Json::UInt64(request.step_duration_ms);
		return json_data;
	}

	inline manager::StepperMotorRequest json_value_to_stepper_request(const Json::Value& json_request) {
		auto retrieve_required_field = [](const Json::Value& json, const std::string& field_name) {
			if (!json.isMember(field_name)) {
				throw std::runtime_error("missing field: " + std::string(field_name));
			}
			return json[field_name];
		};
		return manager::StepperMotorRequest {
			.direction = static_cast<manager::Direction>(retrieve_required_field(json_request, "direction").asInt()),
			.steps_number = static_cast<std::size_t>(retrieve_required_field(json_request, "steps_number").asUInt()),
			.step_duration_ms = static_cast<std::size_t>(retrieve_required_field(json_request, "step_duration_ms").asUInt()),
		};
	}
}

#endif // STEPPER_IPC_DATA_INFRA_HPP