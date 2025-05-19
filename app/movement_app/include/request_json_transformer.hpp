#ifndef	REQUEST_JSON_TRANSFORMER_HPP
#define	REQUEST_JSON_TRANSFORMER_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <string>

#include "json/value.h"

#include "init_request.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "linear_movement_request.hpp"
#include "rotation_movement_request.hpp"
#include "movement_manager_vector.hpp"
#include "ipc_instance.hpp"

namespace ipc {
	template <typename AxesConfig>
	class RequestJsonTransformer {
	public:
		using AxesConfigToJsonTransformer = std::function<Json::Value(const AxesConfig&)>;
		using JsonToAxesConfigTransformer = std::function<AxesConfig(const Json::Value&)>;
		RequestJsonTransformer(
			const AxesConfigToJsonTransformer& ctrlr_cfg_to_json,
			const JsonToAxesConfigTransformer& json_cfg_to_ctrlr
		);
		RequestJsonTransformer(const RequestJsonTransformer&) = default;
		RequestJsonTransformer& operator=(const RequestJsonTransformer&) = default;
		virtual ~RequestJsonTransformer() noexcept = default;

		Json::Value request_to_json_value(const manager::MovementManagerRequest& request) const;
		Instance<manager::MovementManagerRequest> json_value_to_request(const Json::Value& json_request) const;
	private:
		AxesConfigToJsonTransformer m_ctrlr_cfg_to_json;
		JsonToAxesConfigTransformer m_json_cfg_to_ctrlr;
		
		static Json::Value linear_request_to_json_value(const manager::LinearMovementRequest& data);
		static Json::Value rotational_request_to_json_value(const manager::RotationMovementRequest& data);
		Json::Value init_request_to_json_value(const manager::InitRequest<AxesConfig>& data) const;
		
		template <typename T>
		static Json::Value vector_to_json_value(const manager::Vector<T>& data);

		template <typename T> 
		static const T& downcast_request(const manager::MovementManagerRequest& request);
		static Json::Value retrieve_required_field(const Json::Value& json, const std::string& field_name);
		static manager::MovementManagerRequest::RequestType json_value_to_movement_type(const Json::Value json_value);
		static manager::LinearMovementRequest json_value_to_linear_movement_request(const Json::Value json_value);
		manager::InitRequest<AxesConfig> json_value_to_init_request(const Json::Value json_value) const;
		static manager::Vector<double> json_value_to_vector(const Json::Value& json);
	};

	template <typename AxesConfig>
	inline RequestJsonTransformer<AxesConfig>::RequestJsonTransformer(
		const AxesConfigToJsonTransformer& ctrlr_cfg_to_json,
		const JsonToAxesConfigTransformer& json_cfg_to_ctrlr
	): m_ctrlr_cfg_to_json(ctrlr_cfg_to_json), m_json_cfg_to_ctrlr(json_cfg_to_ctrlr) {
		if (!m_ctrlr_cfg_to_json || !m_json_cfg_to_ctrlr) {
			throw std::invalid_argument("invalid transformer(s) received");
		}
	}

	template <typename AxesConfig>
	inline Json::Value RequestJsonTransformer<AxesConfig>::request_to_json_value(const manager::MovementManagerRequest& request) const {
		using RequestType = manager::MovementManagerRequest::RequestType;
		using LinearMovementData = manager::LinearMovementRequest;
		using RotationalMovementData = manager::RotationMovementRequest;

		const auto movement_type = request.type();
		Json::Value json_data;
		json_data["type"] = Json::Int(static_cast<int>(movement_type));
		switch (movement_type) {
		case RequestType::INIT:
			json_data["init_data"] = init_request_to_json_value(downcast_request<manager::InitRequest<AxesConfig>>(request));
			break;
		case RequestType::LINEAR_MOVEMENT:
			json_data["movement_data"] = linear_request_to_json_value(downcast_request<manager::LinearMovementRequest>(request));
			break;
		case RequestType::ROTATIONAL_MOVEMENT:
			json_data["movement_data"] = rotational_request_to_json_value(downcast_request<manager::RotationMovementRequest>(request));
			break;
		default:
			throw std::invalid_argument("unsupported movement type");
		}
		return json_data;
	}

	template <typename AxesConfig>
	inline Instance<manager::MovementManagerRequest> RequestJsonTransformer<AxesConfig>::json_value_to_request(const Json::Value& json_request) const {
		using RequestType = manager::MovementManagerRequest::RequestType;
		const auto request_type = json_value_to_movement_type(retrieve_required_field(json_request, "type"));
		switch (request_type) {
		case RequestType::INIT:
			return Instance<manager::MovementManagerRequest>(
				new manager::InitRequest<AxesConfig>(
					json_value_to_init_request(
						retrieve_required_field(
							json_request,
							"init_data"
						)
					)
				)
			);
		case RequestType::LINEAR_MOVEMENT:
			return Instance<manager::MovementManagerRequest>(
				new manager::LinearMovementRequest(
					json_value_to_linear_movement_request(
						retrieve_required_field(
							json_request,
							"movement_data"
						)
					)
				)
			);
		default:
			throw std::invalid_argument("unsupported movement type");
		}
	}

	template <typename AxesConfig>
	inline Json::Value RequestJsonTransformer<AxesConfig>::linear_request_to_json_value(const manager::LinearMovementRequest& data) {
		Json::Value json_data;
		json_data["destination"] = vector_to_json_value(data.destination());
		json_data["speed"] = data.speed();
		return json_data;
	}

	template <typename AxesConfig>
	inline Json::Value RequestJsonTransformer<AxesConfig>::rotational_request_to_json_value(const manager::RotationMovementRequest& data) {
		Json::Value json_data;
		json_data["rotation_center"] = vector_to_json_value(data.rotation_center());
		json_data["angle"] = data.angle();
		json_data["speed"] = data.speed();
		return json_data;
	}

	template <typename AxesConfig>
	inline Json::Value RequestJsonTransformer<AxesConfig>::init_request_to_json_value(const manager::InitRequest<AxesConfig>& data) const {
		return m_ctrlr_cfg_to_json(data.axis_controller_config());
	}

	template <typename AxesConfig>
	template <typename T>
	inline Json::Value RequestJsonTransformer<AxesConfig>::vector_to_json_value(const manager::Vector<T>& data) {
		Json::Value json_data;
		json_data["x"] = data.get(manager::Axis::X);
		json_data["y"] = data.get(manager::Axis::Y);
		json_data["z"] = data.get(manager::Axis::Z);
		return json_data;
	}

	template <typename AxesConfig>
	template <typename T>
	inline const T& RequestJsonTransformer<AxesConfig>::downcast_request(const manager::MovementManagerRequest& request) {
		try {
			return dynamic_cast<const T&>(request);
		} catch (...) {
			throw std::invalid_argument("failed to cast received request into target type");
		}
	}

	template <typename AxesConfig>
	inline Json::Value RequestJsonTransformer<AxesConfig>::retrieve_required_field(const Json::Value& json, const std::string& field_name) {
		if (!json.isMember(field_name)) {
			throw std::runtime_error("missing field: " + std::string(field_name));
		}
		return json[field_name];
	};

	template <typename AxesConfig>
	inline manager::MovementManagerRequest::RequestType RequestJsonTransformer<AxesConfig>::json_value_to_movement_type(const Json::Value json_value) {
		using RequestType = manager::MovementManagerRequest::RequestType;
		const auto movement_type_mapping = std::map<int, RequestType>{
			{static_cast<int>(RequestType::INIT), RequestType::INIT},
			{static_cast<int>(RequestType::LINEAR_MOVEMENT), RequestType::LINEAR_MOVEMENT},
			{static_cast<int>(RequestType::ROTATIONAL_MOVEMENT), RequestType::ROTATIONAL_MOVEMENT},
		};
		if (!json_value.isInt()) {
			throw std::invalid_argument("movement type json value has wrong format");
		}
		const auto iter = movement_type_mapping.find(json_value.asInt());
		if (movement_type_mapping.end() == iter) {
			throw std::invalid_argument("unsupported int value for RequestType: " + std::to_string(json_value.asInt()));
		}
		return iter->second;
	}

	template <typename AxesConfig>
	inline manager::LinearMovementRequest RequestJsonTransformer<AxesConfig>::json_value_to_linear_movement_request(const Json::Value json_value) {
		return manager::LinearMovementRequest {
			json_value_to_vector(retrieve_required_field(json_value, "destination")),
			retrieve_required_field(json_value, "speed").asFloat(),
		};
	}

	template <typename AxesConfig>
	inline manager::InitRequest<AxesConfig> RequestJsonTransformer<AxesConfig>::json_value_to_init_request(const Json::Value json_value) const {
		return manager::InitRequest<AxesConfig>(m_json_cfg_to_ctrlr(json_value));
	}

	template <typename AxesConfig>
	inline manager::Vector<double> RequestJsonTransformer<AxesConfig>::json_value_to_vector(const Json::Value& json) {
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
}

#endif // REQUEST_JSON_TRANSFORMER_HPP