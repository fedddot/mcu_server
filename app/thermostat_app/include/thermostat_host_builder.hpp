#ifndef	THERMOSTAT_HOST_BUILDER_HPP
#define	THERMOSTAT_HOST_BUILDER_HPP

#include <exception>
#include <functional>
#include <optional>
#include <stdexcept>

#include "api_request_reader_builder.hpp"
#include "api_response_writer_builder.hpp"
#include "host.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "ipc_instance.hpp"
#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"
#include "thermostat_manager.hpp"
#include "thermostat_manager_controller.hpp"
#include "thermostat_vendor.hpp"


namespace host {
	template <typename RawData>
	class ThermostatHostBuilder {
	public:
		using ApiRequest = vendor::ThermostatVendorApiRequest;
		using ApiRequestParser = std::function<ApiRequest(const RawData&)>;
		using RawDataReader = ipc::IpcDataReader<RawData>;

		using ApiResponse = vendor::ThermostatVendorApiResponse;
		using ApiResponseSerializer = std::function<RawData(const ApiResponse&)>;
		using RawDataWriter = ipc::IpcDataWriter<RawData>;

		using Controller = manager::ThermostatManagerController;
		
		ThermostatHostBuilder(): m_raw_reader_ptr(nullptr), m_raw_writer_ptr(nullptr), m_controller_ptr(nullptr) {

		}
		ThermostatHostBuilder(const ThermostatHostBuilder&) = default;
		ThermostatHostBuilder& operator=(const ThermostatHostBuilder&) = default;
		virtual ~ThermostatHostBuilder() noexcept = default;
		
		Host<ApiRequest, ApiResponse> build() const {
			const auto api_request_reader = m_api_request_reader_builder.build();
			const auto api_response_writer = m_api_response_writer_builder.build();
			if (!m_controller_ptr) {
				throw std::runtime_error("controller_ptr has not been set");
			}
			const auto thermostat_manager_instance = vendor::ThermostatVendor::ThermostatManagerInstance(new manager::ThermostatManager(m_controller_ptr));
			const auto thermostat_vendor = Host<ApiRequest, ApiResponse>::VendorInstance(new vendor::ThermostatVendor(thermostat_manager_instance));
			return Host<ApiRequest, ApiResponse>(
				api_request_reader,
				api_response_writer,
				thermostat_vendor,
				[](const std::exception& e) {
					return ApiResponse(
						ApiResponse::Result::FAILURE,
						std::string("an exception caught at host: ") + std::string(e.what())
					);
				}
			);
		}
		ThermostatHostBuilder& set_api_request_parser(const ApiRequestParser& api_request_parser) {
			m_api_request_reader_builder.set_api_request_parser(api_request_parser);
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_raw_data_reader(const RawDataReader& raw_data_reader) {
			m_api_request_reader_builder.set_raw_data_reader(raw_data_reader);
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_raw_data_writer(const RawDataWriter& raw_data_writer) {
			m_api_response_writer_builder.set_raw_data_writer(raw_data_writer);
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_api_response_serializer(const ApiResponseSerializer& api_response_serializer) {
			m_api_response_writer_builder.set_api_response_serializer(api_response_serializer);
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_controller(Controller *controller_ptr) {
			if (!controller_ptr) {
				throw std::invalid_argument("invalid controller_ptr reveived");
			}
			m_controller_ptr = controller_ptr;
			return std::ref(*this);
		}
	private:
		RawDataReader *m_raw_reader_ptr;
		RawDataWriter *m_raw_writer_ptr;
		std::optional<ApiRequestParser> m_parser_opt;
		std::optional<ApiResponseSerializer> m_serializer_opt;

		Controller *m_controller_ptr;
	};
}

#endif // THERMOSTAT_HOST_BUILDER_HPP