#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <optional>
#include <stdexcept>

#include "service.hpp"

namespace host {
	template <typename ApiRequest, typename ApiResponse>
	class Host {
	public:
		using ApiRequestReader = std::function<std::optional<ApiRequest>(void)>;
		using ApiResponseWriter = std::function<void(const ApiResponse&)>;
		using FailureReporter = std::function<ApiResponse(const std::exception&)>;
		using ServiceInstance = service::Service<ApiRequest, ApiResponse>;

		Host(
			const ApiRequestReader& api_request_reader,
			const ApiResponseWriter& api_response_writer,
			const FailureReporter& failure_reporter,
			ServiceInstance *service_ptr
		);
		Host(const Host&) = default;
		Host& operator=(const Host&) = default;
		virtual ~Host() noexcept = default;
		
		void run_once();
	private:
		ApiRequestReader m_api_request_reader;
		ApiResponseWriter m_api_response_writer;
		FailureReporter m_failure_reporter;
		ServiceInstance *m_service_ptr;
	};

	template <typename ApiRequest, typename ApiResponse>
	inline Host<ApiRequest, ApiResponse>::Host(
		const ApiRequestReader& api_request_reader,
		const ApiResponseWriter& api_response_writer,
		const FailureReporter& failure_reporter,
		ServiceInstance *service_ptr
	): m_api_request_reader(api_request_reader), m_api_response_writer(api_response_writer), m_failure_reporter(failure_reporter), m_service_ptr(service_ptr) {
		if (!m_api_request_reader || !m_api_response_writer || !m_failure_reporter || !m_service_ptr) {
			throw std::invalid_argument("invalid args received in Host ctor");
		}
	}

	template <typename ApiRequest, typename ApiResponse>
	inline void Host<ApiRequest, ApiResponse>::run_once() {
		try {
			const auto request = m_api_request_reader();
			if (!request.has_value()) {
				return;
			}
			const auto response = m_service_ptr->run_api_request(request.value());
			m_api_response_writer(response);
		} catch (const std::exception& e) {
			const auto response = m_failure_reporter(e);
			m_api_response_writer(response);
		}
	}
}

#endif // HOST_HPP