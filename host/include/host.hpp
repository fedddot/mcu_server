#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

#include "ipc_connection.hpp"
#include "manager.hpp"
#include "providers.hpp"

namespace host {
	template <typename Trequest, typename Tresponse, typename Tprovider_id, typename Tproviders_cfg, typename Tmanager_cfg, typename Tipc_config>
	class Host {
	public:
		using IpcConnection = ipc::IpcConnection<Trequest, Tresponse>;
		using IpcFactory = std::function<IpcConnection *(const Tipc_config&)>;
		using Manager = manager::Manager<Trequest, Tresponse, Tprovider_id>;
		using Providers = manager::Providers<Tprovider_id>;
		using ManagerFactory = std::function<Manager *(Providers *, const Tmanager_cfg&)>;
		using ProvidersFactory = std::function<Providers *(const Tproviders_cfg&)>;
		using FailureReporter = std::function<Tresponse(const std::exception&)>;
		Host(
			const IpcFactory& ipc_factory,
			const Tipc_config& ipc_config,
			const ManagerFactory& manager_factory,
			const Tmanager_cfg& manager_config,
			const ProvidersFactory& providers_factory,
			const Tproviders_cfg& providers_config,
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

		std::unique_ptr<IpcConnection> m_ipc_connection;
		std::unique_ptr<Providers> m_providers;
		std::unique_ptr<Manager> m_manager;
	};

	template <typename Trequest, typename Tresponse, typename Tprovider_id, typename Tproviders_cfg, typename Tmanager_cfg, typename Tipc_config>
	inline Host<Trequest, Tresponse, Tprovider_id, Tproviders_cfg, Tmanager_cfg, Tipc_config>::Host(
		const IpcFactory& ipc_factory,
		const Tipc_config& ipc_config,
		const ManagerFactory& manager_factory,
		const Tmanager_cfg& manager_config,
		const ProvidersFactory& providers_factory,
		const Tproviders_cfg& providers_config,
		const FailureReporter& failure_reporter
	): m_failure_reporter(failure_reporter), m_is_running(false) {
		if (!ipc_factory || !manager_factory || !providers_factory || !failure_reporter) {
			throw std::invalid_argument("invalid host args");
		}
		m_ipc_connection = std::unique_ptr<IpcConnection>(ipc_factory(ipc_config));
		m_providers = std::unique_ptr<Providers>(providers_factory(providers_config));
		m_manager = std::unique_ptr<Manager>(manager_factory(m_providers.get(), manager_config));
	}

	template <typename Trequest, typename Tresponse, typename Tprovider_id, typename Tproviders_cfg, typename Tmanager_cfg, typename Tipc_config>
	inline void Host<Trequest, Tresponse, Tprovider_id, Tproviders_cfg, Tmanager_cfg, Tipc_config>::run_once() {
		try {
			if (!m_ipc_connection->readable()) {
				return;
			}
			const auto request = m_ipc_connection->read();
			const auto response = m_manager->run(request);
			m_ipc_connection->write(response);
		} catch (const std::exception& e) {
			m_ipc_connection->write(m_failure_reporter(e));
		}
	}

	template <typename Trequest, typename Tresponse, typename Tprovider_id, typename Tproviders_cfg, typename Tmanager_cfg, typename Tipc_config>
	inline void Host<Trequest, Tresponse, Tprovider_id, Tproviders_cfg, Tmanager_cfg, Tipc_config>::run() {
		m_is_running = true;
		while (m_is_running) {
			run_once();
		}
	}

	template <typename Trequest, typename Tresponse, typename Tprovider_id, typename Tproviders_cfg, typename Tmanager_cfg, typename Tipc_config>
	inline bool Host<Trequest, Tresponse, Tprovider_id, Tproviders_cfg, Tmanager_cfg, Tipc_config>::is_running() const {
		return m_is_running;
	}

	template <typename Trequest, typename Tresponse, typename Tprovider_id, typename Tproviders_cfg, typename Tmanager_cfg, typename Tipc_config>
	inline void Host<Trequest, Tresponse, Tprovider_id, Tproviders_cfg, Tmanager_cfg, Tipc_config>::stop() {
		m_is_running = false;
	}
}

#endif // HOST_HPP