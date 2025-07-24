#ifndef	THERMOSTAT_HOST_BUILDER_HPP
#define	THERMOSTAT_HOST_BUILDER_HPP

#include <exception>
#include <functional>
#include <optional>
#include <stdexcept>

#include "host.hpp"
#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"

namespace host {
	template <typename RawData>
	class ThermostatHostBuilder {
	public:
		using ApiRequest = service::ThermostatApiRequest;
		using ApiRequestParser = std::function<ApiRequest(const RawData&)>;
		using RawDataReader = std::function<std::optional<RawData>(void)>;

		using ApiResponse = service::ThermostatServiceApiResponse;
		using ApiResponseSerializer = std::function<RawData(const ApiResponse&)>;
		using RawDataWriter = std::function<void(const RawData&)>;

		using Service = Host<ApiRequest, ApiResponse>::ServiceInstance;
		
		ThermostatHostBuilder(): m_service_ptr(nullptr) {

		}
		ThermostatHostBuilder(const ThermostatHostBuilder&) = default;
		ThermostatHostBuilder& operator=(const ThermostatHostBuilder&) = default;
		virtual ~ThermostatHostBuilder() noexcept = default;
		
		Host<ApiRequest, ApiResponse> build() const {
			return Host<ApiRequest, ApiResponse>(
				[](void) -> std::optional<ApiRequest> {
					throw std::runtime_error("NOT IMPLEMENTED");
				},
				[](const ApiResponse&) {
					throw std::runtime_error("NOT IMPLEMENTED");
				},
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
		ThermostatHostBuilder& set_raw_data_reader(const RawDataReader& raw_data_reader) {
			m_raw_reader = raw_data_reader;
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
		RawDataReader m_raw_reader;
		RawDataWriter m_raw_writer;
		ApiRequestParser m_parser;
		ApiResponseSerializer m_serializer;
		Service *m_service_ptr;
	};
}

#endif // THERMOSTAT_HOST_BUILDER_HPP