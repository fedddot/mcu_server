#ifndef	HOST_HPP
#define	HOST_HPP

#include <functional>
#include <stdexcept>

#include "ipc_server.hpp"
#include "manager.hpp"

namespace host {
	template <typename Request, typename Response>
	class Host {
	public:
		using FailureReporter = std::function<Response(const std::exception&)>;
		Host(
			ipc::IpcServer<Request, Response> *ipc_server,
			manager::Manager<Request, Response> *manager,
			const FailureReporter& failure_reporter
		);
		Host(const Host&) = delete;
		Host& operator=(const Host&) = delete;
		virtual ~Host() noexcept = default;
		
		void run_once() const;
	private:
		ipc::IpcServer<Request, Response> *m_ipc_server;
		manager::Manager<Request, Response> *m_manager;
		const FailureReporter m_failure_reporter;
		const FailureReporter m_m_failure_reporter;
	};

	template <typename Request, typename Response>
	inline Host<Request, Response>::Host(
		ipc::IpcServer<Request, Response> *ipc_server,
		manager::Manager<Request, Response> *manager,
		const FailureReporter& failure_reporter
	): m_ipc_server(ipc_server), m_manager(manager), m_failure_reporter(failure_reporter) {
		if (!m_ipc_server || !m_manager || !m_failure_reporter) {
			throw std::invalid_argument("invalid host args received");
		}
	}

	template <typename Request, typename Response>
	inline void Host<Request, Response>::run_once() const {
		m_ipc_server->serve_once(
			[this](const Request& request) {
				return m_manager->run(request);
			}
		);
	}
}

#endif // HOST_HPP