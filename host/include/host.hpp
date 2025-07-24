#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <stdexcept>

#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "service.hpp"
#include "service_instance.hpp"

namespace host {
	template <typename ApiRequest, typename ApiResponse>
	class Host {
	public:
		using ApiRequestReaderInstance = ipc::Instance<ipc::IpcDataReader<ApiRequest>>;
		using ApiResponseWriterInstance = ipc::Instance<ipc::IpcDataWriter<ApiResponse>>;
		using ServiceInstance = service::Instance<service::Service<ApiRequest, ApiResponse>>;
		using FailureReporter = std::function<ApiResponse(const std::exception&)>;

		Host(
			const ApiRequestReaderInstance& api_request_reader,
			const ApiResponseWriterInstance& api_response_writer,
			const ServiceInstance& service,
			const FailureReporter& failure_reporter
		);
		Host(const Host&) = default;
		Host& operator=(const Host&) = default;
		virtual ~Host() noexcept = default;
		
		void run_once();
	private:
		ApiRequestReaderInstance m_api_request_reader;
		ApiResponseWriterInstance m_api_response_writer;
		ServiceInstance m_service;
		FailureReporter m_failure_reporter;
	};

	template <typename ApiRequest, typename ApiResponse>
	inline Host<ApiRequest, ApiResponse>::Host(
		const ApiRequestReaderInstance& api_request_reader,
		const ApiResponseWriterInstance& api_response_writer,
		const ServiceInstance& service,
		const FailureReporter& failure_reporter
	): m_api_request_reader(api_request_reader), m_api_response_writer(api_response_writer), m_service(service), m_failure_reporter(failure_reporter) {
		if (!m_failure_reporter) {
			throw std::invalid_argument("invalid failure reporter received");
		}
	}

	template <typename ApiRequest, typename ApiResponse>
	inline void Host<ApiRequest, ApiResponse>::run_once() {
		try {
			const auto request = m_api_request_reader.get().read();
			if (!request) {
				return;
			}
			const auto response = m_service.get().run_api_request(request->get());
			m_api_response_writer.get().write(response);
		} catch (const std::exception& e) {
			const auto response = m_failure_reporter(e);
			m_api_response_writer.get().write(response);
		}
	}
}

#endif // HOST_HPP