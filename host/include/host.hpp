#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <stdexcept>

#include "vendor.hpp"
#include "vendor_api_request.hpp"
#include "vendor_api_response.hpp"
#include "vendor_instance.hpp"
#include "ipc_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"

namespace host {
	template <typename ManagerId, typename Payload>
	class Host {
	public:
		using ApiRequest = vendor::ApiRequest<ManagerId, Payload>;
		using ApiResponse = vendor::ApiResponse;
		using FailureReporter = std::function<vendor::Instance<ApiResponse>(const std::exception&)>;
		Host(
			const ipc::Instance<ipc::IpcDataReader<ApiRequest>>& ipc_data_reader,
			const ipc::Instance<ipc::IpcDataWriter<ApiResponse>>& ipc_data_writer,
			const vendor::Instance<vendor::Vendor<ManagerId, Payload>>& vendor,
			const FailureReporter& failure_reporter
		);
		Host(const Host&) = delete;
		Host& operator=(const Host&) = delete;
		virtual ~Host() noexcept = default;
		
		void run_once();
	private:
		ipc::Instance<ipc::IpcDataReader<ApiRequest>> m_ipc_data_reader;
		ipc::Instance<ipc::IpcDataWriter<ApiResponse>> m_ipc_data_writer;
		vendor::Instance<vendor::Vendor<ManagerId, Payload>> m_vendor;
		FailureReporter m_failure_reporter;
	};

	template <typename ManagerId, typename Payload>
	inline Host<ManagerId, Payload>::Host(
		const ipc::Instance<ipc::IpcDataReader<ApiRequest>>& ipc_data_reader,
		const ipc::Instance<ipc::IpcDataWriter<ApiResponse>>& ipc_data_writer,
		const vendor::Instance<vendor::Vendor<ManagerId, Payload>>& vendor,
		const FailureReporter& failure_reporter
	): m_ipc_data_reader(ipc_data_reader), m_ipc_data_writer(ipc_data_writer), m_vendor(vendor), m_failure_reporter(failure_reporter) {
		if (!m_failure_reporter) {
			throw std::invalid_argument("invalid failure reporter received");
		}
	}

	template <typename ManagerId, typename Payload>
	inline void Host<ManagerId, Payload>::run_once() {
		try {
			const auto request = m_ipc_data_reader.get().read();
			if (!request) {
				return;
			}
			const auto response = m_vendor.get().run(request->get());
			m_ipc_data_writer.get().write(response);
		} catch (const std::exception& e) {
			const auto response = m_failure_reporter(e);
			m_ipc_data_writer.get().write(response.get());
		}
	}
}

#endif // HOST_HPP