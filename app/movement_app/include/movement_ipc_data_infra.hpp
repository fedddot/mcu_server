#ifndef	MOVEMENT_IPC_DATA_INFRA_HPP
#define	MOVEMENT_IPC_DATA_INFRA_HPP

#include <cstddef>
#include <vector>

#include "json/value.h"

#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"

namespace ipc {
	using RawData = std::vector<char>;
	
	inline Json::Value movement_response_to_json_value(const manager::StepperMotorResponse& response) {
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

	inline manager::StepperMotorResponse json_value_to_movement_response(const Json::Value& json_response) {
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

	inline Json::Value movement_request_to_json_value(const manager::StepperMotorRequest& request) {
		Json::Value json_data;
		json_data["motor_id"] = Json::String(request.motor_id);
		json_data["direction"] = Json::Int(static_cast<int>(request.direction));
		json_data["steps_number"] = Json::UInt64(request.steps_number);
		json_data["step_duration_ms"] = Json::UInt64(request.step_duration_ms);
		return json_data;
	}

	inline manager::StepperMotorRequest json_value_to_movement_request(const Json::Value& json_request) {
		auto retrieve_required_field = [](const Json::Value& json, const std::string& field_name) {
			if (!json.isMember(field_name)) {
				throw std::runtime_error("missing field: " + std::string(field_name));
			}
			return json[field_name];
		};
		return manager::StepperMotorRequest {
			.motor_id = retrieve_required_field(json_request, "motor_id").asString(),
			.direction = static_cast<manager::Direction>(retrieve_required_field(json_request, "direction").asInt()),
			.steps_number = static_cast<std::size_t>(retrieve_required_field(json_request, "steps_number").asUInt()),
			.step_duration_ms = static_cast<std::size_t>(retrieve_required_field(json_request, "step_duration_ms").asUInt()),
		};
	}
}

#endif // MOVEMENT_IPC_DATA_INFRA_HPP