#ifndef	THERMOSTAT_HOST_BUILDER_HPP
#define	THERMOSTAT_HOST_BUILDER_HPP

#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <optional>
#include <stdexcept>

#include "api_request_reader.hpp"
#include "api_response_writer.hpp"
#include "host.hpp"
#include "ipc_queue.hpp"
#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"
#include "thermostat_controller.hpp"
#include "thermostat_service.hpp"

namespace host {
	template <std::size_t HSIZE>
	class ThermostatApp {
	public:
		using ApiRequest = service::ThermostatApiRequest;
		using SizeRetriever = typename ipc::ApiRequestReader<ApiRequest, HSIZE>::SizeRetriever;
		using ApiRequestParser = typename ipc::ApiRequestReader<ApiRequest, HSIZE>::ApiRequestParser;
		
		using ApiResponse = service::ThermostatServiceApiResponse;
		using ApiResponseSerializer = typename ipc::ApiResponseWriter<ApiResponse, HSIZE>::ApiResponseSerializer;
		using HeaderGenerator = typename ipc::ApiResponseWriter<ApiResponse, HSIZE>::HeaderGenerator;
		using RawDataWriter = typename ipc::ApiResponseWriter<ApiResponse, HSIZE>::RawDataWriter;
		
		using RawDataQueue = ipc::IpcQueue<std::uint8_t>;
		using ThermostatController = service::ThermostatController;
		
		ThermostatApp(): m_api_request_reader(std::nullopt), m_api_response_writer(std::nullopt), m_service(std::nullopt), m_raw_queue(nullptr), m_controller_ptr(nullptr) {}
		ThermostatApp(const ThermostatApp&) = default;
		ThermostatApp& operator=(const ThermostatApp&) = default;
		virtual ~ThermostatApp() noexcept = default;
		
		void build() {
			build_reader();
			build_writer();
			m_service.emplace(m_controller_ptr);
			m_host.emplace(
				&m_api_request_reader.value(),
				&m_api_response_writer.value(),
				[](const std::exception& e) {
					return ApiResponse(
						ApiResponse::Result::FAILURE,
						std::string("an exception caught at host: ") + std::string(e.what())
					);
				},
				&m_service.value()
			);
		}
		ThermostatApp& set_package_size_retriever(const SizeRetriever& size_retriever) {
			m_size_retriever = size_retriever;
			return std::ref(*this);
		}
		ThermostatApp& set_api_request_parser(const ApiRequestParser& api_request_parser) {
			m_api_request_parser = api_request_parser;
			return std::ref(*this);
		}
		ThermostatApp& set_api_response_serializer(const ApiResponseSerializer& api_response_serializer) {
			m_api_response_serializer = api_response_serializer;
			return std::ref(*this);
		}
		ThermostatApp& set_header_generator(const HeaderGenerator& header_generator) {
			m_header_generator = header_generator;
			return std::ref(*this);
		}
		ThermostatApp& set_raw_data_writer(const RawDataWriter& raw_data_writer) {
			m_raw_writer = raw_data_writer;
			return std::ref(*this);
		}
		ThermostatApp& set_raw_data_queue(RawDataQueue *raw_data_queue_ptr) {
			m_raw_queue = raw_data_queue_ptr;
			return std::ref(*this);
		}
		ThermostatApp& set_thermostat_controller(ThermostatController *controller_ptr) {
			m_controller_ptr = controller_ptr;
			return std::ref(*this);
		}
	private:
		SizeRetriever m_size_retriever;
		ApiRequestParser m_api_request_parser;
		ApiResponseSerializer m_api_response_serializer;
		HeaderGenerator m_header_generator;
		RawDataWriter m_raw_writer;
		RawDataQueue *m_raw_queue;
		ThermostatController *m_controller_ptr;
		
		std::optional<ipc::ApiRequestReader<ApiRequest, HSIZE>> m_api_request_reader;
		std::optional<ipc::ApiResponseWriter<ApiResponse, HSIZE>> m_api_response_writer;
		std::optional<service::ThermostatService> m_service;
		std::optional<Host<ApiRequest, ApiResponse>> m_host;
		
		void build_reader() {
			if (!m_raw_queue || !m_size_retriever || !m_api_request_parser) {
				throw std::runtime_error("failed build reader: one or more required components are not set");
			}
			m_api_request_reader.emplace(
				m_raw_queue,
				m_size_retriever,
				m_api_request_parser
			);
		}

		void build_writer() {
			if (!m_header_generator || !m_api_response_serializer || !m_raw_writer) {
				throw std::runtime_error("failed build writer: one or more required components are not set");
			}
			m_api_response_writer.emplace(
				m_header_generator,
				m_raw_writer,
				m_api_response_serializer
			);
		}
	};
}

#endif // THERMOSTAT_HOST_BUILDER_HPP