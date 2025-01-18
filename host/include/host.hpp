#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

#include "ipc_config.hpp"
#include "ipc_server.hpp"
#include "manager.hpp"
#include "manager_config.hpp"

namespace host {
	template <typename Request, typename Response>
	class Host {
	public:
		using IpcFactory = std::function<ipc::IpcServer<Request, Response> *(const ipc::IpcConfig&)>;
		using ManagerFactory = std::function<manager::Manager<Request, Response> *(const manager::ManagerConfig&)>;
		using FailureReporter = std::function<Response(const std::exception&)>;
		Host(
			const IpcFactory& ipc_factory,
			const ipc::IpcConfig& ipc_config,
			const ManagerFactory& manager_factory,
			const manager::ManagerConfig& manager_config,
			const FailureReporter& failure_reporter
		);
		virtual ~Host() noexcept = default;
		void run_once();
		void run();
		bool is_running() const;
		void stop();
	private:
		FailureReporter m_failure_reporter;
		bool m_is_running;

		using IpcServer = ipc::IpcServer<Request, Response>;
		using Manager = manager::Manager<Request, Response>;

		std::unique_ptr<IpcServer> m_ipc_server;
		std::unique_ptr<Manager> m_manager;
	};

	template <typename Request, typename Response>
	inline Host<Request, Response>::Host(
		const IpcFactory& ipc_factory,
		const ipc::IpcConfig& ipc_config,
		const ManagerFactory& manager_factory,
		const manager::ManagerConfig& manager_config,
		const FailureReporter& failure_reporter
	): m_failure_reporter(failure_reporter), m_is_running(false) {
		if (!ipc_factory || !manager_factory || !failure_reporter) {
			throw std::invalid_argument("invalid host args");
		}
		m_ipc_server = std::unique_ptr<IpcServer>(ipc_factory(ipc_config));
		m_manager = std::unique_ptr<Manager>(manager_factory(manager_config));
	}

	template <typename Request, typename Response>
	inline void Host<Request, Response>::run_once() {
		try {
			const auto request_option = m_ipc_server->read();
			if (!request_option.some()) {
				return;
			}
			const auto response = m_manager->run(request_option.get());
			m_ipc_server->write(response);
		} catch (const std::exception& e) {
			m_ipc_server->write(m_failure_reporter(e));
		}
	}

	template <typename Request, typename Response>
	inline void Host<Request, Response>::run() {
		m_is_running = true;
		while (m_is_running) {
			run_once();
		}
	}

	template <typename Request, typename Response>
	inline bool Host<Request, Response>::is_running() const {
		return m_is_running;
	}

	template <typename Request, typename Response>
	inline void Host<Request, Response>::stop() {
		m_is_running = false;
	}
}

#endif // HOST_HPP