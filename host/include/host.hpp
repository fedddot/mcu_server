#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

#include "manager.hpp"
#include "manager_clonable.hpp"
#include "ipc_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"

namespace host {
	template <typename Request, typename Response>
	class Host {
	public:
		using FailureReporter = std::function<Response(const std::exception&)>;
		Host(
			const ipc::Instance<ipc::IpcDataReader<Request>>& ipc_data_reader,
			const ipc::Instance<ipc::IpcDataWriter<Response>>& ipc_data_writer,
			const manager::Clonable<manager::Manager<Request, Response>>& manager,
			const FailureReporter& failure_reporter
		);
		Host(const Host&) = delete;
		Host& operator=(const Host&) = delete;
		virtual ~Host() noexcept = default;
		
		void run_once();
	private:
		ipc::Instance<ipc::IpcDataReader<Request>> m_ipc_data_reader;
		const ipc::Instance<ipc::IpcDataWriter<Response>> m_ipc_data_writer;
		const std::unique_ptr<manager::Manager<Request, Response>> m_manager;
		const FailureReporter m_failure_reporter;
	};

	template <typename Request, typename Response>
	inline Host<Request, Response>::Host(
		const ipc::Instance<ipc::IpcDataReader<Request>>& ipc_data_reader,
		const ipc::Instance<ipc::IpcDataWriter<Response>>& ipc_data_writer,
		const manager::Clonable<manager::Manager<Request, Response>>& manager,
		const FailureReporter& failure_reporter
	): m_ipc_data_reader(ipc_data_reader), m_ipc_data_writer(ipc_data_writer), m_manager(manager.clone()), m_failure_reporter(failure_reporter) {
		if (!m_failure_reporter) {
			throw std::invalid_argument("invalid failure reporter received");
		}
	}

	template <typename Request, typename Response>
	inline void Host<Request, Response>::run_once() {
		try {
			const auto request = m_ipc_data_reader.get().read();
			if (!request) {
				return;
			}
			const auto response = m_manager->run(request->get());
			m_ipc_data_writer.get().write(response);
		} catch (const std::exception& e) {
			const auto response = m_failure_reporter(e);
			m_ipc_data_writer.get().write(response);
		}
	}
}

#endif // HOST_HPP