#ifndef	MOVEMENT_IPC_DATA_INFRA_HPP
#define	MOVEMENT_IPC_DATA_INFRA_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "json/value.h"

#include "init_request.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "linear_movement_request.hpp"
#include "rotation_movement_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_manager_vector.hpp"

namespace ipc {
	template <typename AxisControllerConfig>
	class DefaultMovementDataTransformers {
	public:
		using AxisControllerConfigToJsonTransformer = std::function<Json::Value(const AxisControllerConfig&)>;
		using JsonToAxisControllerConfigTransformer = std::function<AxisControllerConfig(const Json::Value&)>;
		DefaultMovementDataTransformers(
			const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json,
			const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr
		);
		DefaultMovementDataTransformers(const DefaultMovementDataTransformers&) = default;
		DefaultMovementDataTransformers& operator=(const DefaultMovementDataTransformers&) = default;
		virtual ~DefaultMovementDataTransformers() noexcept = default;

		Json::Value request_to_json_value(const manager::MovementManagerRequest& request) const;
		Json::Value response_to_json_value(const manager::MovementManagerResponse& response) const;
	private:
		AxisControllerConfigToJsonTransformer m_ctrlr_cfg_to_json;
		JsonToAxisControllerConfigTransformer m_json_cfg_to_ctrlr;
		
		static Json::Value linear_request_to_json_value(const manager::LinearMovementRequest& data);
		static Json::Value rotational_movement_data_to_json_value(const manager::RotationMovementRequest& data);
		
		template <typename T>
		static Json::Value vector_to_json_value(const manager::Vector<T>& data);

		template <typename T> 
		static const T& downcast_request(const manager::MovementManagerRequest& request);
	};

	template <typename AxisControllerConfig>
	inline DefaultMovementDataTransformers<AxisControllerConfig>::DefaultMovementDataTransformers(
		const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json,
		const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr
	): m_ctrlr_cfg_to_json(ctrlr_cfg_to_json), m_json_cfg_to_ctrlr(json_cfg_to_ctrlr) {
		if (!m_ctrlr_cfg_to_json || !m_json_cfg_to_ctrlr) {
			throw std::invalid_argument("invalid transformer(s) received");
		}
	}

	template <typename AxisControllerConfig>
	inline Json::Value DefaultMovementDataTransformers<AxisControllerConfig>::request_to_json_value(const manager::MovementManagerRequest& request) const {
		using RequestType = manager::MovementManagerRequest::RequestType;
		using LinearMovementData = manager::LinearMovementRequest;
		using RotationalMovementData = manager::RotationMovementRequest;

		const auto movement_type = request.type();
		Json::Value json_data;
		json_data["type"] = Json::Int(static_cast<int>(movement_type));
		switch (movement_type) {
		case RequestType::LINEAR_MOVEMENT:
			json_data["movement_data"] = linear_movement_request_to_json_value(downcast_request<manager::LinearMovementRequest>(request));
			break;
		case RequestType::ROTATIONAL_MOVEMENT:
			json_data["movement_data"] = rotational_movement_data_to_json_value(downcast_request<manager::RotationMovementRequest>(request));
			break;
		default:
			throw std::invalid_argument("unsupported movement type");
		}
		return json_data;
	}

	template <typename AxisControllerConfig>
	inline Json::Value DefaultMovementDataTransformers<AxisControllerConfig>::linear_request_to_json_value(const manager::LinearMovementRequest& data) {
		Json::Value json_data;
		json_data["destination"] = vector_to_json_value(data.destination());
		json_data["speed"] = data.speed();
		return json_data;
	}

	template <typename AxisControllerConfig>
	inline Json::Value DefaultMovementDataTransformers<AxisControllerConfig>::rotational_movement_data_to_json_value(const manager::RotationMovementRequest& data) {
		Json::Value json_data;
		json_data["rotation_center"] = vector_to_json_value(data.rotation_center());
		json_data["angle"] = data.angle();
		json_data["speed"] = data.speed();
		return json_data;
	}

	template <typename AxisControllerConfig>
	template <typename T>
	inline Json::Value DefaultMovementDataTransformers<AxisControllerConfig>::vector_to_json_value(const manager::Vector<T>& data) {
		Json::Value json_data;
		json_data["x"] = data.get(manager::Axis::X);
		json_data["y"] = data.get(manager::Axis::Y);
		json_data["z"] = data.get(manager::Axis::Z);
		return json_data;
	}

	template <typename AxisControllerConfig>
	template <typename T>
	inline const T& DefaultMovementDataTransformers<AxisControllerConfig>::downcast_request(const manager::MovementManagerRequest& request) {
		try {
			return dynamic_cast<const T&>(request);
		} catch (...) {
			throw std::invalid_argument("failed to cast received request into target type");
		}
	}

	template <typename AxisControllerConfig>
	inline Json::Value DefaultMovementDataTransformers<AxisControllerConfig>::response_to_json_value(const manager::MovementManagerResponse& response) const {
		Json::Value json_data;
		json_data["result"] = static_cast<int>(response.code);
		if (response.message) {
			json_data["message"] = response.message.value();
		}
		return json_data;
	}

	// inline manager::MovementManagerResponse json_value_to_movement_response(const Json::Value& json_response) {
	// 	manager::MovementManagerResponse response;
	// 	if (!json_response.isMember("result")) {
	// 		throw std::runtime_error("Invalid JSON response: missing 'result' field");
	// 	}
	// 	response.code = static_cast<manager::MovementManagerResponse::ResultCode>(json_response["result"].asInt());
	// 	if (json_response.isMember("message")) {
	// 		response.message = json_response["message"].asString();
	// 	}
	// 	return response;
	// }



	

	// inline Json::Value init_request_to_json_value(const manager::InitRequest<AxisControllerConfig>& data) {
	// 	Json::Value json_data;
	// 	json_data["destination"] = vector_to_json_value(data.destination());
	// 	json_data["speed"] = data.speed();
	// 	return json_data;
	// }





	

	// inline manager::MovementManagerRequest::RequestType json_value_to_movement_type(const Json::Value json_value) {
	// 	using RequestType = manager::MovementManagerRequest::RequestType;
	// 	const auto movement_type_mapping = std::map<int, RequestType>{
	// 		{static_cast<int>(RequestType::LINEAR), RequestType::LINEAR},
	// 		{static_cast<int>(RequestType::ROTATIONAL), RequestType::ROTATIONAL},
	// 	};
	// 	if (!json_value.isInt()) {
	// 		throw std::invalid_argument("movement type json value has wrong format");
	// 	}
	// 	return movement_type_mapping.at(json_value.asInt());
	// }

	// inline Json::Value retrieve_required_field(const Json::Value& json, const std::string& field_name) {
	// 	if (!json.isMember(field_name)) {
	// 		throw std::runtime_error("missing field: " + std::string(field_name));
	// 	}
	// 	return json[field_name];
	// };

	// inline manager::Vector<double> json_value_to_vector(const Json::Value& json) {
	// 	using namespace manager;
	// 	const auto axis_tags_mapping = std::map<Axis, std::string>{
	// 		{Axis::X, "x"},
	// 		{Axis::Y, "y"},
	// 		{Axis::Z, "z"},
	// 	};
	// 	auto result = Vector<double>(0.0, 0.0, 0.0);
	// 	for (const auto& [axis, tag]: axis_tags_mapping) {
	// 		const auto projection = retrieve_required_field(json, tag);
	// 		if (!projection.isDouble()) {
	// 			throw std::invalid_argument(tag + " vector projection has wrong format");
	// 		}
	// 		result.set(axis, projection.asDouble());
	// 	}
	// 	return result;
	// };

	// inline manager::LinearMovementRequest json_value_to_linear_movement_request(const Json::Value json_value) {
	// 	return manager::LinearMovementRequest {
	// 		json_value_to_vector(retrieve_required_field(json_value, "destination")),
	// 		retrieve_required_field(json_value, "speed").asFloat(),
	// 	};
	// }

	// inline manager::MovementManagerRequest json_value_to_movement_request(const Json::Value& json_request) {
	// 	using RequestType = manager::MovementManagerRequest::RequestType;
	// 	const auto request_type = json_value_to_movement_type(retrieve_required_field(json_request, "type"));
	// 	switch (request_type) {
	// 	case RequestType::LINEAR:
	// 		return manager::MovementManagerRequest(json_value_to_linear_movement_request(retrieve_required_field(json_request, "movement_data")));
	// 	default:
	// 		throw std::invalid_argument("unsupported movement type");
	// 	}
	// }
}

#endif // MOVEMENT_IPC_DATA_INFRA_HPP