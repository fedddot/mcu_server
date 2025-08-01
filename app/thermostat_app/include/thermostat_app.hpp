#ifndef	THERMOSTAT_HOST_BUILDER_HPP
#define	THERMOSTAT_HOST_BUILDER_HPP

#include <cstddef>
#include <cstdint>
#include <exception>
#include <optional>

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
		
		ThermostatApp(
			const SizeRetriever& size_retriever,
			const ApiRequestParser& api_request_parser,
			const ApiResponseSerializer& api_response_serializer,
			const HeaderGenerator& header_generator,
			const RawDataWriter& raw_writer,
			RawDataQueue *raw_queue,
			ThermostatController *controller_ptr
		): m_api_request_reader(raw_queue, size_retriever, api_request_parser), m_api_response_writer(header_generator, raw_writer, api_response_serializer), m_service(std::nullopt), m_host(std::nullopt) {
			m_service.emplace(controller_ptr);
			m_host.emplace(
				&m_api_request_reader,
				&m_api_response_writer,
				[](const std::exception& e) {
					return ApiResponse(
						ApiResponse::Result::FAILURE,
						std::string("an exception caught at host: ") + std::string(e.what())
					);
				},
				&m_service.value()
			);
		}
		ThermostatApp(const ThermostatApp&) = default;
		ThermostatApp& operator=(const ThermostatApp&) = default;
		virtual ~ThermostatApp() noexcept = default;
		
		void run_once() {
			m_host.value().run_once();
		}
	private:		
		ipc::ApiRequestReader<ApiRequest, HSIZE> m_api_request_reader;
		ipc::ApiResponseWriter<ApiResponse, HSIZE> m_api_response_writer;
		std::optional<service::ThermostatService> m_service;
		std::optional<Host<ApiRequest, ApiResponse>> m_host;
	};
}

#endif // THERMOSTAT_HOST_BUILDER_HPP