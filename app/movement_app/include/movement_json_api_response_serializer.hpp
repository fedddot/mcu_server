#ifndef MOVEMENT_JSON_API_RESPONSE_SERIALIZER_HPP
#define MOVEMENT_JSON_API_RESPONSE_SERIALIZER_HPP

#include <string>
#include "json/value.h"
#include "movement_vendor_api_response.hpp"

namespace ipc {
    class MovementJsonApiResponseSerializer {
    public:
        MovementJsonApiResponseSerializer() = default;
        MovementJsonApiResponseSerializer(const MovementJsonApiResponseSerializer&) = default;
        MovementJsonApiResponseSerializer& operator=(const MovementJsonApiResponseSerializer&) = default;
        virtual ~MovementJsonApiResponseSerializer() noexcept = default;

        Json::Value operator()(const vendor::MovementVendorApiResponse& response) const;
    };

    inline Json::Value MovementJsonApiResponseSerializer::operator()(const vendor::MovementVendorApiResponse& response) const {
        const auto result_mapping = std::map<vendor::MovementVendorApiResponse::Result, std::string>{
            {vendor::MovementVendorApiResponse::Result::SUCCESS, "SUCCESS"},
            {vendor::MovementVendorApiResponse::Result::FAILURE, "FAILURE"},
        };
        Json::Value json;
        json["status"] = result_mapping.at(response.result());
        if (response.message()) {
            json["message"] = *response.message();
        }
        return json;
    }
}

#endif // MOVEMENT_JSON_API_RESPONSE_SERIALIZER_HPP