#ifndef	THERMOSTAT_HOST_BUILDER_HPP
#define	THERMOSTAT_HOST_BUILDER_HPP

#include <exception>
#include <optional>
#include <stdexcept>

#include "api_request_reader_builder.hpp"
#include "api_response_writer_builder.hpp"
#include "host.hpp"
#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "ipc_instance.hpp"
#include "manager_instance.hpp"
#include "relay_controller.hpp"
#include "temperature_sensor_controller.hpp"
#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"
#include "thermostat_manager.hpp"
#include "thermostat_vendor.hpp"
#include "timer_scheduler.hpp"

namespace host {
	class ThermostatHostBuilder {
	public:
		using ApiRequest = vendor::ThermostatVendorApiRequest;
		using ApiRequestParser = typename ipc::ApiRequestReaderBuilder<ApiRequest, ipc::RawData>::ApiRequestParser;
		using RawDataReaderInstance = ipc::Instance<ipc::IpcDataReader<ipc::RawData>>;

		using ApiResponse = vendor::ThermostatVendorApiResponse;
		using ApiResponseSerializer = typename ipc::ApiResponseWriterBuilder<ApiResponse, ipc::RawData>::ApiResponseSerializer;
		using RawDataWriterInstance = ipc::Instance<ipc::IpcDataWriter<ipc::RawData>>;

		using RelayControllerInstance = manager::Instance<manager::RelayController>;
		using SchedulerInstance = manager::Instance<manager::TimerScheduler>;
		using SensorInstance = manager::Instance<manager::TemperatureSensorController>;

		using FailureReporter = typename Host<ApiRequest, ApiResponse>::FailureReporter;
		
		ThermostatHostBuilder() = default;
		ThermostatHostBuilder(const ThermostatHostBuilder&) = default;
		ThermostatHostBuilder& operator=(const ThermostatHostBuilder&) = default;
		virtual ~ThermostatHostBuilder() noexcept = default;
		
		Host<ApiRequest, ApiResponse> build() const {
			const auto api_request_reader = m_api_request_reader_builder.build();
			const auto api_response_writer = m_api_response_writer_builder.build();
			const auto thermostat_manager_instance = vendor::ThermostatVendor::ThermostatManagerInstance(
				new manager::ThermostatManager(
					retrieve_from_option(m_relay_controller, "relay controller instance"),
					retrieve_from_option(m_sensor, "temp sensor controller instance"),
					retrieve_from_option(m_scheduler, "scheduler instance")
				)
			);
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
		ThermostatHostBuilder& set_relay_controller(const RelayControllerInstance& relay_controller) {
			m_relay_controller = relay_controller;
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_scheduler(const SchedulerInstance& scheduler) {
			m_scheduler = scheduler;
			return std::ref(*this);
		}
		ThermostatHostBuilder& set_temp_sensor(const SensorInstance& temp_sensor) {
			m_sensor = temp_sensor;
			return std::ref(*this);
		}
	private:
		ipc::ApiRequestReaderBuilder<ApiRequest, ipc::RawData> m_api_request_reader_builder;
		ipc::ApiResponseWriterBuilder<ApiResponse, ipc::RawData> m_api_response_writer_builder;
		std::optional<RelayControllerInstance> m_relay_controller;
		std::optional<SchedulerInstance> m_scheduler;
		std::optional<SensorInstance> m_sensor;
		

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