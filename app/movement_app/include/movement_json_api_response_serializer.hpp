#ifndef MOVEMENT_JSON_API_RESPONSE_SERIALIZER_HPP
#define MOVEMENT_JSON_API_RESPONSE_SERIALIZER_HPP

#include <functional>
#include "json/value.h"
#include "movement_vendor_api_response.hpp"

namespace ipc {
    class MovementJsonApiResponseSerializer {
    public:
        using Serializer = std::function<Json::Value(const vendor::MovementVendorApiResponse&)>;

        MovementJsonApiResponseSerializer() = default;
        MovementJsonApiResponseSerializer(const MovementJsonApiResponseSerializer&) = default;
        MovementJsonApiResponseSerializer& operator=(const MovementJsonApiResponseSerializer&) = default;
        virtual ~MovementJsonApiResponseSerializer() noexcept = default;

        // Example: serialize a response to JSON
        Json::Value operator()(const vendor::MovementVendorApiResponse& response) const {
            // Placeholder: implement actual serialization logic as needed
            Json::Value json;
            json["status"] = static_cast<int>(response.status());
            json["message"] = response.message();
            return json;
        }
    };
}

#endif // MOVEMENT_JSON_API_RESPONSE_SERIALIZER_HPP