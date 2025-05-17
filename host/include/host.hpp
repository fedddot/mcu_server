#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <stdexcept>

#include "host_instance.hpp"
#include "vendor.hpp"
#include "vendor_api_request.hpp"
#include "vendor_api_response.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"

namespace host {
	template <typename ManagerId, typename Payload>
	class Host {
	public:
		using ApiRequest = vendor::ApiRequest<ManagerId, Payload>;
		using ApiRequestReaderInstance = host::Instance<ipc::IpcDataReader<ApiRequest>>;
		using ApiResponse = vendor::ApiResponse;
		using ApiResponseWriterInstance = host::Instance<ipc::IpcDataWriter<ApiResponse>>;
		using VendorInstance = host::Instance<vendor::Vendor<ManagerId, Payload>>;
		using FailureReporter = std::function<host::Instance<ApiResponse>(const std::exception&)>;

		Host(
			const ApiRequestReaderInstance& api_request_reader,
			const ApiResponseWriterInstance& api_response_writer,
			const VendorInstance& vendor,
			const FailureReporter& failure_reporter
		);
		Host(const Host&) = delete;
		Host& operator=(const Host&) = delete;
		virtual ~Host() noexcept = default;
		
		void run_once();
	private:
		ApiRequestReaderInstance m_api_request_reader;
		ApiResponseWriterInstance m_api_response_writer;
		VendorInstance m_vendor;
		FailureReporter m_failure_reporter;
	};

	template <typename ManagerId, typename Payload>
	inline Host<ManagerId, Payload>::Host(
		const ApiRequestReaderInstance& api_request_reader,
		const ApiResponseWriterInstance& api_response_writer,
		const VendorInstance& vendor,
		const FailureReporter& failure_reporter
	): m_api_request_reader(api_request_reader), m_api_response_writer(api_response_writer), m_vendor(vendor), m_failure_reporter(failure_reporter) {
		if (!m_failure_reporter) {
			throw std::invalid_argument("invalid failure reporter received");
		}
	}

	template <typename ManagerId, typename Payload>
	inline void Host<ManagerId, Payload>::run_once() {
		try {
			const auto request = m_api_request_reader.get().read();
			if (!request) {
				return;
			}
			const auto response = m_vendor.get().run_api_request(request->get());
			m_api_response_writer.get().write(response.get());
		} catch (const std::exception& e) {
			const auto response = m_failure_reporter(e);
			m_api_response_writer.get().write(response.get());
		}
	}
}

#endif // HOST_HPP