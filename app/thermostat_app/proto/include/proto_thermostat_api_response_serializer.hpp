#ifndef PROTO_THERMOSTAT_API_RESPONSE_SERIALIZER_HPP
#define PROTO_THERMOSTAT_API_RESPONSE_SERIALIZER_HPP

#include <map>
#include <stdexcept>
#include <vector>

#include "pb.h"
#include "pb_encode.h"

#include "thermostat_api_response.hpp"
#include "service_api.pb.h"

namespace ipc {
    class ApiResponseSerializer {
    public:
        ApiResponseSerializer() = default;
        ApiResponseSerializer(const ApiResponseSerializer&) = default;
        ApiResponseSerializer& operator=(const ApiResponseSerializer&) = default;
        virtual ~ApiResponseSerializer() noexcept = default;

        std::vector<std::uint8_t> operator()(const service::ThermostatServiceApiResponse& response) const;
    private:
        static bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    };

    inline std::vector<std::uint8_t> ApiResponseSerializer::operator()(const service::ThermostatServiceApiResponse& response) const {
        using namespace service;
        using ServiceStatus = service::ThermostatServiceApiResponse::Result;
        const auto status_mapping = std::map<ServiceStatus, service_api_StatusCode> {
            { ServiceStatus::SUCCESS, service_api_StatusCode_SUCCESS },
            { ServiceStatus::FAILURE, service_api_StatusCode_FAILURE },
        };
        const auto pb_status = status_mapping.at(response.result());
        const auto pb_msg = response.message() ? response.message().value() : std::string("");
        const auto pb_temperature = response.temperature() ? static_cast<float>(response.temperature().value()) : 0.0f;
        const auto pb_response = service_api_ThermostatApiResponse {
            .status = pb_status,
            .message = pb_callback_t {
                .funcs = {
                    .encode = &encode_string,
                },
                .arg = const_cast<std::string *>(&pb_msg),
            },
            .current_temperature = pb_temperature
        };

        enum: int { BUFF_SIZE = 512UL };
        pb_byte_t buffer[BUFF_SIZE];
        pb_ostream_t ostream = pb_ostream_from_buffer(
            buffer,
            BUFF_SIZE
        );
        if (!pb_encode(&ostream, service_api_ThermostatApiResponse_fields, &pb_response)) {
            throw std::runtime_error("failed to encode ThermostatServiceApiResponse into protocol buffer: " + std::string(PB_GET_ERROR(&ostream)));
        }
        return std::vector<std::uint8_t>((const char *)buffer, (const char *)buffer + ostream.bytes_written);
    }

    inline bool ApiResponseSerializer::encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
        if (!arg || !*arg) {
            throw std::runtime_error("encode_string called with null arg");
        }
        const auto str = *static_cast<const std::string *>(*arg);
        if (!pb_encode_tag_for_field(stream, field)) {
            return false;
        }
        return pb_encode_string(stream, (const pb_byte_t *)(str.c_str()), str.size());
    }
}

#endif // PROTO_THERMOSTAT_API_RESPONSE_SERIALIZER_HPP