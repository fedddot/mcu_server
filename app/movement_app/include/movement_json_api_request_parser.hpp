#ifndef MOVEMENT_JSON_API_REQUEST_PARSER_HPP
#define MOVEMENT_JSON_API_REQUEST_PARSER_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <string>

#include "json/value.h"

#include "axes_controller_config_request.hpp"
#include "ipc_instance.hpp"
#include "movement_vendor_api_request.hpp"

namespace ipc {
    template <typename AxesConfig>
    class MovementJsonApiRequestParser {
    public:
        using AxesConfigParser = std::function<AxesConfig(const Json::Value&)>;
        
        MovementJsonApiRequestParser(const AxesConfigParser& axes_config_parser);
        MovementJsonApiRequestParser(const MovementJsonApiRequestParser&) = default;
        MovementJsonApiRequestParser& operator=(const MovementJsonApiRequestParser&) = default;
        virtual ~MovementJsonApiRequestParser() noexcept = default;

        Instance<vendor::MovementVendorApiRequest> parse(const Json::Value& json_value) const;
    private:
        using RequestType = typename vendor::MovementVendorApiRequest::RequestType;
        AxesConfigParser m_axes_config_parser;

        static RequestType parse_request_type(const Json::Value& json_value);
        Instance<vendor::MovementVendorApiRequest> parse_config_request(const Json::Value& json_value) const;
        static Instance<vendor::MovementVendorApiRequest> parse_linear_movement_request(const Json::Value& json_value);
        static Instance<vendor::MovementVendorApiRequest> parse_rotational_movement_request(const Json::Value& json_value);
        static Instance<vendor::MovementVendorApiRequest> parse_movement_vendor_api_request(const Json::Value& json_value);
        static std::string retrieve_string(const Json::Value& json_value, const std::string& key);
    };

    template <typename AxesConfig>
    inline MovementJsonApiRequestParser<AxesConfig>::MovementJsonApiRequestParser(const AxesConfigParser& axes_config_parser): m_axes_config_parser(axes_config_parser) {
        if (!m_axes_config_parser) {
            throw std::invalid_argument("invalid axes config parser received");
        }
    }

    template <typename AxesConfig>
    inline Instance<vendor::MovementVendorApiRequest> MovementJsonApiRequestParser<AxesConfig>::parse(const Json::Value& json_value) const {
        const auto request_type = parse_request_type(json_value);
        switch (request_type) {
        case RequestType::CONFIG:
            return parse_config_request(json_value);
        case RequestType::LINEAR_MOVEMENT:
            return parse_linear_movement_request(json_value);
        case RequestType::ROTATIONAL_MOVEMENT:
            return parse_rotational_movement_request(json_value);
        default:
            throw std::invalid_argument("unsupported request type");
        }
    }

    template <typename AxesConfig>
    inline typename MovementJsonApiRequestParser<AxesConfig>::RequestType MovementJsonApiRequestParser<AxesConfig>::parse_request_type(const Json::Value& json_value) {
        const auto request_type_mapping = std::map<std::string, RequestType>{
            {"CONFIG",              RequestType::CONFIG},
            {"LINEAR_MOVEMENT",     RequestType::LINEAR_MOVEMENT},
            {"ROTATIONAL_MOVEMENT", RequestType::ROTATIONAL_MOVEMENT}
        };
        const auto request_type = retrieve_string(json_value, "request_type");
        const auto iter = request_type_mapping.find(request_type);
        if (iter == request_type_mapping.end()) {
            throw std::invalid_argument("unsupported request type: " + request_type);
        }
        return iter->second;
    }

    template <typename AxesConfig>
    inline Instance<vendor::MovementVendorApiRequest> MovementJsonApiRequestParser<AxesConfig>::parse_config_request(const Json::Value& json_value) const {
        const auto axes_config = m_axes_config_parser(json_value);
        return Instance<vendor::MovementVendorApiRequest>(
            new vendor::AxesControllerConfigApiRequest<AxesConfig>(axes_config)
        );
    }

    template <typename AxesConfig>
    inline Instance<vendor::MovementVendorApiRequest> MovementJsonApiRequestParser<AxesConfig>::parse_linear_movement_request(const Json::Value& json_value) {
        throw std::runtime_error("NOT IMPLEMENTED");
    }

    template <typename AxesConfig>
    inline Instance<vendor::MovementVendorApiRequest> MovementJsonApiRequestParser<AxesConfig>::parse_rotational_movement_request(const Json::Value& json_value) {
        throw std::runtime_error("NOT IMPLEMENTED");
    }

    template <typename AxesConfig>
    inline Instance<vendor::MovementVendorApiRequest> MovementJsonApiRequestParser<AxesConfig>::parse_movement_vendor_api_request(const Json::Value& json_value) {
        throw std::runtime_error("NOT IMPLEMENTED");
    }

    template <typename AxesConfig>
    inline std::string MovementJsonApiRequestParser<AxesConfig>::retrieve_string(const Json::Value& json_value, const std::string& key) {
        if (!json_value.isMember(key)) {
            throw std::invalid_argument("received json data is missing key: " + key);
        }
        const auto val = json_value[key];
        if (!val.isString()) {
            throw std::invalid_argument("wrong data format for key: " + key);
        }
        return val.asString();
    }
}

#endif // MOVEMENT_JSON_API_REQUEST_PARSER_HPP