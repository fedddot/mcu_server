#ifndef	THERMOSTAT_HOST_BUILDER_HPP
#define	THERMOSTAT_HOST_BUILDER_HPP

#include <exception>
#include <optional>
#include <stdexcept>

#include "api_request_reader_builder.hpp"
#include "api_response_writer_builder.hpp"
#include "host.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "ipc_instance.hpp"
#include "thermostat_manager.hpp"
#include "thermostat_vendor.hpp"
#include "thermostat_vendor_api_request.hpp"
#include "thermostat_vendor_api_response.hpp"

namespace host {
	template <typename AxesConfig, typename RawData>
	class ThermostatHostBuilder {
	public:
		using ApiRequest = vendor::ThermostatVendorApiRequest;
		using ApiRequestParser = typename ipc::ApiRequestReaderBuilder<ApiRequest, RawData>::ApiRequestParser;
		using RawDataReaderInstance = ipc::Instance<ipc::IpcDataReader<RawData>>;

		using ApiResponse = vendor::ThermostatVendorApiResponse;
		using ApiResponseSerializer = typename ipc::ApiResponseWriterBuilder<ApiResponse, RawData>::ApiResponseSerializer;
		using RawDataWriterInstance = ipc::Instance<ipc::IpcDataWriter<RawData>>;

		using AxesControllerCreator = typename manager::ThermostatManager<AxesConfig>::AxesControllerCreator;

		using FailureReporter = typename Host<ApiRequest, ApiResponse>::FailureReporter;
		
		ThermostatHostBuilder() = default;
		ThermostatHostBuilder(const ThermostatHostBuilder&) = default;
		ThermostatHostBuilder& operator=(const ThermostatHostBuilder&) = default;
		virtual ~ThermostatHostBuilder() noexcept = default;
		
		Host<ApiRequest, ApiResponse> build() const {
			const auto api_request_reader = m_api_request_reader_builder.build();
			const auto api_response_writer = m_api_response_writer_builder.build();
			const auto thermostat_manager_instance = typename vendor::ThermostatVendor<AxesConfig>::ThermostatManagerInstance(
				new manager::ThermostatManager<AxesConfig>(
					retrieve_from_option(m_axes_controller_ctor, "axes controller constructor")
				)
			);
			const auto thermostat_vendor = Host<ApiRequest, ApiResponse>::VendorInstance(new vendor::ThermostatVendor<AxesConfig>(thermostat_manager_instance));
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
		ThermostatHostBuilder& set_raw_data_reader(const RawDataReaderInstance& raw_data_reader) {
			m_api_request_reader_builder.set_raw_data_reader(raw_data_reader);
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_raw_data_writer(const RawDataWriterInstance& raw_data_writer) {
			m_api_response_writer_builder.set_raw_data_writer(raw_data_writer);
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_api_response_serializer(const ApiResponseSerializer& api_response_serializer) {
			m_api_response_writer_builder.set_api_response_serializer(api_response_serializer);
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_axes_controller_creator(const AxesControllerCreator& axes_controller_ctor) {
			m_axes_controller_ctor = axes_controller_ctor;
			return std::ref(*this);
		}
	private:
		ipc::ApiRequestReaderBuilder<ApiRequest, RawData> m_api_request_reader_builder;
		ipc::ApiResponseWriterBuilder<ApiResponse, RawData> m_api_response_writer_builder;
		std::optional<AxesControllerCreator> m_axes_controller_ctor;

		template <typename T>
		static const T& retrieve_from_option(const std::optional<T>& option, const std::string& option_name) {
			if (!option) {
				throw std::runtime_error(option_name + " has not been set");
			}
			return std::ref(*option);
		}
	};
}

#endif // THERMOSTAT_HOST_BUILDER_HPP