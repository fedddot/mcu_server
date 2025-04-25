#ifndef	MOVEMENT_IPC_DATA_INFRA_HPP
#define	MOVEMENT_IPC_DATA_INFRA_HPP

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "json/value.h"

#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_manager_vector.hpp"

namespace ipc {
	using RawData = std::vector<char>;
	
	inline Json::Value movement_response_to_json_value(const manager::MovementManagerResponse& response) {
		Json::Value json_data;
		json_data["result"] = static_cast<int>(response.code);
		if (response.message) {
			json_data["message"] = response.message.value();
		}
		return json_data;
	}

	inline manager::MovementManagerResponse json_value_to_movement_response(const Json::Value& json_response) {
		manager::MovementManagerResponse response;
		if (!json_response.isMember("result")) {
			throw std::runtime_error("Invalid JSON response: missing 'result' field");
		}
		response.code = static_cast<manager::MovementManagerResponse::ResultCode>(json_response["result"].asInt());
		if (json_response.isMember("message")) {
			response.message = json_response["message"].asString();
		}
		return response;
	}

	template <typename T>
	inline Json::Value vector_to_json_value(const manager::Vector<T>& data) {
		Json::Value json_data;
		json_data["x"] = data.get(manager::Axis::X);
		json_data["y"] = data.get(manager::Axis::Y);
		json_data["z"] = data.get(manager::Axis::Z);
		return json_data;
	}

	inline Json::Value linear_movement_data_to_json_value(const manager::MovementManagerRequest::LinearMovementData& data) {
		Json::Value json_data;
		json_data["destination"] = vector_to_json_value(data.destination);
		json_data["speed"] = data.speed;
		return json_data;
	}

	inline Json::Value rotational_movement_data_to_json_value(const manager::MovementManagerRequest::RotationalMovementData& data) {
		Json::Value json_data;
		json_data["rotation_center"] = vector_to_json_value(data.rotation_center);
		json_data["angle"] = data.angle;
		json_data["speed"] = data.speed;
		return json_data;
	}

	inline Json::Value movement_request_to_json_value(const manager::MovementManagerRequest& request) {
		using MovementType = manager::MovementManagerRequest::MovementType;
		using LinearMovementData = manager::MovementManagerRequest::LinearMovementData;
		using RotationalMovementData = manager::MovementManagerRequest::RotationalMovementData;

		const auto movement_type = request.get_movement_type();
		Json::Value json_data;
		json_data["type"] = Json::Int(static_cast<int>(movement_type));
		switch (movement_type) {
		case MovementType::LINEAR:
			json_data["movement_data"] = linear_movement_data_to_json_value(request.get_movement_data<LinearMovementData>());
			break;
		case MovementType::ROTATIONAL:
			json_data["movement_data"] = rotational_movement_data_to_json_value(request.get_movement_data<RotationalMovementData>());
			break;
		default:
			throw std::invalid_argument("unsupported movement type");
		}
		return json_data;
	}

	inline manager::MovementManagerRequest::MovementType json_value_to_movement_type(const Json::Value json_value) {
		using MovementType = manager::MovementManagerRequest::MovementType;
		const auto movement_type_mapping = std::map<int, MovementType>{
			{static_cast<int>(MovementType::LINEAR), MovementType::LINEAR},
			{static_cast<int>(MovementType::ROTATIONAL), MovementType::ROTATIONAL},
		};
		if (!json_value.isInt()) {
			throw std::invalid_argument("movement type json value has wrong format");
		}
		return movement_type_mapping.at(json_value.asInt());
	}

	inline Json::Value retrieve_required_field(const Json::Value& json, const std::string& field_name) {
		if (!json.isMember(field_name)) {
			throw std::runtime_error("missing field: " + std::string(field_name));
		}
		return json[field_name];
	};

	inline manager::Vector<double> json_value_to_vector(const Json::Value& json) {
		using namespace manager;
		const auto axis_tags_mapping = std::map<Axis, std::string>{
			{Axis::X, "x"},
			{Axis::Y, "y"},
			{Axis::Z, "z"},
		};
		auto result = Vector<double>(0.0, 0.0, 0.0);
		for (const auto& [axis, tag]: axis_tags_mapping) {
			const auto projection = retrieve_required_field(json, tag);
			if (!projection.isDouble()) {
				throw std::invalid_argument(tag + " vector projection has wrong format");
			}
			result.set(axis, projection.asDouble());
		}
		return result;
	};

	inline manager::MovementManagerRequest::LinearMovementData json_value_to_linear_movement_data(const Json::Value json_value) {
		return manager::MovementManagerRequest::LinearMovementData {
			json_value_to_vector(retrieve_required_field(json_value, "destination")),
			retrieve_required_field(json_value, "speed").asFloat(),
		};
	}

	inline manager::MovementManagerRequest json_value_to_movement_request(const Json::Value& json_request) {
		using MovementType = manager::MovementManagerRequest::MovementType;
		const auto request_type = json_value_to_movement_type(retrieve_required_field(json_request, "type"));
		switch (request_type) {
		case MovementType::LINEAR:
			return manager::MovementManagerRequest(json_value_to_linear_movement_data(retrieve_required_field(json_request, "movement_data")));
		default:
			throw std::invalid_argument("unsupported movement type");
		}
	}
}

#endif // MOVEMENT_IPC_DATA_INFRA_HPP