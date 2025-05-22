#ifndef MOVEMENT_JSON_API_REQUEST_PARSER_HPP
#define MOVEMENT_JSON_API_REQUEST_PARSER_HPP

#include <map>
#include <stdexcept>

#include "json/value.h"
#include <string>

#include "ipc_instance.hpp"
#include "movement_vendor_api_request.hpp"

namespace ipc {
    template <typename AxesConfig>
    class MovementJsonApiRequestParser {
    public:
        MovementJsonApiRequestParser() = default;
        MovementJsonApiRequestParser(const MovementJsonApiRequestParser&) = default;
        MovementJsonApiRequestParser& operator=(const MovementJsonApiRequestParser&) = default;
        virtual ~MovementJsonApiRequestParser() = default;

		Instance<vendor::MovementVendorApiRequest> parse(const Json::Value& json_value) const {
			const auto request_type = parse_request_type(json_value);
			switch (request_type) {
			case RequestType::CONFIG:
				return parse_config_request(json_value);
			case RequestType::LINEAR_MOVEMENT:
				return parse_linear_movement_request(json_value);
			case RequestType::ROTATIONAL_MOVEMENT:
				return parse_rotational_movement_request(json_value);
			default:
				throw std::runtime_error("Unknown request type");
			}
		}

	private:
		using RequestType = vendor::MovementVendorApiRequest::RequestType;
		
		static RequestType parse_request_type(const Json::Value& json_value) {
			const auto request_type_mapping = std::map<std::string, RequestType>{
				{"CONFIG",				RequestType::CONFIG},
				{"LINEAR_MOVEMENT",		RequestType::LINEAR_MOVEMENT},
				{"ROTATIONAL_MOVEMENT", RequestType::ROTATIONAL_MOVEMENT}
			};
			const auto request_type = retrieve_string(json_value, "request_type");
			const auto iter = request_type_mapping.find(request_type);
			if (iter == request_type_mapping.end()) {
				throw std::invalid_argument("unsupported request type: " + request_type);
			}
			throw iter->second;
		}

		static Instance<vendor::MovementVendorApiRequest> parse_config_request(const Json::Value& json_value) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}

		static Instance<vendor::MovementVendorApiRequest> parse_linear_movement_request(const Json::Value& json_value) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}

		static Instance<vendor::MovementVendorApiRequest> parse_rotational_movement_request(const Json::Value& json_value) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}

		static Instance<vendor::MovementVendorApiRequest> parse_movement_vendor_api_request(const Json::Value& json_value) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}

		static std::string retrieve_string(const Json::Value& json_value, const std::string& key) {
			if (!json_value.isMember(key)) {
				throw std::invalid_argument("received json data is missing key: " + key);
			}
			const auto& val = json_value[key];
			if (!val.isString()) {
				throw std::invalid_argument("wrong data format for key: " + key);
			}
			return json_value[key].asString();
		}
	};
}
#endif // MOVEMENT_JSON_API_REQUEST_PARSER_HPP