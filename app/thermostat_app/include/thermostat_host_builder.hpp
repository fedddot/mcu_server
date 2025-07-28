#ifndef	THERMOSTAT_HOST_BUILDER_HPP
#define	THERMOSTAT_HOST_BUILDER_HPP

#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <optional>
#include <vector>

#include "api_request_reader.hpp"
#include "data_buffer.hpp"
#include "host.hpp"
#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"

namespace host {
	template <std::size_t EncodedPackageSizeFieldLength = 2>
	class ThermostatHostBuilder {
	public:
		using ApiRequest = service::ThermostatApiRequest;
		using ApiRequestParser = std::function<ApiRequest(const std::vector<std::uint8_t>&)>;
		using RawDataBuffer = host_tools::DataBuffer<std::uint8_t>;

		using ApiResponse = service::ThermostatServiceApiResponse;
		using ApiResponseSerializer = std::function<std::vector<std::uint8_t>(const ApiResponse&)>;
		using RawDataWriter = std::function<void(const std::vector<std::uint8_t>&)>;

		using Service = Host<ApiRequest, ApiResponse>::ServiceInstance;
		
		ThermostatHostBuilder(): m_service_ptr(nullptr) {

		}
		ThermostatHostBuilder(const ThermostatHostBuilder&) = default;
		ThermostatHostBuilder& operator=(const ThermostatHostBuilder&) = default;
		virtual ~ThermostatHostBuilder() noexcept = default;
		
		Host<ApiRequest, ApiResponse> build() const {
			const auto api_request_reader = host_tools::ApiRequestReader<ApiRequest, EncodedPackageSizeFieldLength>(
				m_raw_buffer,
				m_parser
			);
			const auto api_response_writer = [raw_writer = m_raw_writer, serializer = m_serializer](const ApiResponse& response) {
				const auto serialized_response = serializer(response);
				raw_writer(serialized_response);
			};
			return Host<ApiRequest, ApiResponse>(
				api_request_reader,
				api_response_writer,
				[](const std::exception& e) {
					return ApiResponse(
						ApiResponse::Result::FAILURE,
						std::string("an exception caught at host: ") + std::string(e.what())
					);
				},
				m_service_ptr
			);
		}

		ThermostatHostBuilder& set_api_request_parser(const ApiRequestParser& api_request_parser) {
			m_parser = api_request_parser;
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_raw_data_buffer(RawDataBuffer *raw_data_buffer_ptr) {
			m_raw_buffer = raw_data_buffer_ptr;
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_raw_data_writer(const RawDataWriter& raw_data_writer) {
			m_raw_writer = raw_data_writer;
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_api_response_serializer(const ApiResponseSerializer& api_response_serializer) {
			m_serializer = api_response_serializer;
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_service(Service *service_ptr) {
			m_service_ptr = service_ptr;
			return std::ref(*this);
		}
	private:
		RawDataBuffer *m_raw_buffer;
		RawDataWriter m_raw_writer;
		ApiRequestParser m_parser;
		ApiResponseSerializer m_serializer;
		Service *m_service_ptr;
	};
}

#endif // THERMOSTAT_HOST_BUILDER_HPP