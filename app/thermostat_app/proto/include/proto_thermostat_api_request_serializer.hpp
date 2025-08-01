#ifndef PROTO_THERMOSTAT_API_REQUEST_SERIALIZER_HPP
#define PROTO_THERMOSTAT_API_REQUEST_SERIALIZER_HPP

#include <map>
#include <stdexcept>
#include <vector>

#include "pb.h"
#include "pb_encode.h"

#include "thermostat_api_request.hpp"
#include "service_api.pb.h"

namespace ipc {
    class ApiRequestSerializer {
    public:
        ApiRequestSerializer() = default;
        ApiRequestSerializer(const ApiRequestSerializer&) = default;
        ApiRequestSerializer& operator=(const ApiRequestSerializer&) = default;
        virtual ~ApiRequestSerializer() noexcept = default;

        std::vector<std::uint8_t> operator()(const service::ThermostatApiRequest& request) const;
    };

    inline std::vector<std::uint8_t> ApiRequestSerializer::operator()(const service::ThermostatApiRequest& request) const {
        using namespace service;
        using RequestType = service::ThermostatApiRequest::RequestType;
        const auto type_mapping = std::map<RequestType, service_api_RequestType> {
            { RequestType::GET_TEMP, service_api_RequestType_GET_TEMP },
            { RequestType::START, service_api_RequestType_START },
            { RequestType::STOP, service_api_RequestType_STOP },
        };
        const auto pb_type = type_mapping.at(request.type());
        const auto pb_temperature = request.temperature() ? static_cast<float>(request.temperature().value()) : 0.0f;
        const auto pb_time_resolution_ms = request.time_resolution_ms() ? static_cast<uint32_t>(request.time_resolution_ms().value()) : 0;
        const auto pb_request = service_api_ThermostatApiRequest {
            .request_type = pb_type,
            .set_temperature = pb_temperature,
            .time_resolution_ms = pb_time_resolution_ms,
        };

        enum: int { BUFF_SIZE = 256UL };
        pb_byte_t buffer[BUFF_SIZE];
        pb_ostream_t ostream = pb_ostream_from_buffer(
            buffer,
            BUFF_SIZE
        );
        if (!pb_encode(&ostream, service_api_ThermostatApiRequest_fields, &pb_request)) {
            throw std::runtime_error("failed to encode ThermostatServiceApiRequest into protocol buffer: " + std::string(PB_GET_ERROR(&ostream)));
        }
        return std::vector<std::uint8_t>((const char *)buffer, (const char *)buffer + ostream.bytes_written);
    }
}

#endif // PROTO_THERMOSTAT_API_REQUEST_SERIALIZER_HPP