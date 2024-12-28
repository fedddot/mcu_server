#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

#include "ipc_connection.hpp"
#include "manager.hpp"
#include "provider.hpp"

namespace app {
	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg, typename Tipc_config>
	class Host {
	public:
		using IpcFactory = std::function<ipc::IpcConnection<Trequest, Tresponse> *(const Tipc_config&)>;
		using ManagerFactory = std::function<manager::Manager<Trequest, Tresponse> *(provider::Provider *, const Tmanager_cfg&)>;
		using ProviderFactory = std::function<provider::Provider *(const Tprovider_cfg&)>;
		using FailureReporter = std::function<Tresponse(const std::exception&)>;
		Host(
			const IpcFactory& ipc_factory,
			const Tipc_config& ipc_config,
			const ManagerFactory& manager_factory,
			const Tmanager_cfg& manager_config,
			const ProviderFactory& provider_factory,
			const Tprovider_cfg& provider_config,
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

		std::unique_ptr<ipc::IpcConnection<Trequest, Tresponse>> m_ipc_connection;
		std::unique_ptr<provider::Provider> m_provider;
		std::unique_ptr<manager::Manager<Trequest, Tresponse>> m_manager;
	};

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg, typename Tipc_config>
	inline Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg, Tipc_config>::Host(
		const IpcFactory& ipc_factory,
		const Tipc_config& ipc_config,
		const ManagerFactory& manager_factory,
		const Tmanager_cfg& manager_config,
		const ProviderFactory& provider_factory,
		const Tprovider_cfg& provider_config,
		const FailureReporter& failure_reporter
	): m_failure_reporter(failure_reporter), m_is_running(false) {
		using namespace manager;
		using namespace provider;
		using namespace ipc;
		if (!ipc_factory || !manager_factory || !provider_factory || !failure_reporter) {
			throw std::invalid_argument("invalid host args");
		}
		m_ipc_connection = std::unique_ptr<IpcConnection<Trequest, Tresponse>>(ipc_factory(ipc_config));
		m_provider = std::unique_ptr<Provider>(provider_factory(provider_config));
		m_manager = std::unique_ptr<Manager<Trequest, Tresponse>>(manager_factory(m_provider.get(), manager_config));
	}

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg, typename Tipc_config>
	inline void Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg, Tipc_config>::run_once() {
		try {
			if (!m_ipc_connection->reader()->readable()) {
				return;
			}
			const auto request = m_ipc_connection->reader()->read();
			const auto response = m_manager->run(request);
			m_ipc_connection->writer()->write(response);
		} catch (const std::exception& e) {
			m_ipc_connection->writer()->write(m_failure_reporter(e));
		}
	}

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg, typename Tipc_config>
	inline void Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg, Tipc_config>::run() {
		m_is_running = true;
		while (m_is_running) {
			run_once();
		}
	}

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg, typename Tipc_config>
	inline bool Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg, Tipc_config>::is_running() const {
		return m_is_running;
	}

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg, typename Tipc_config>
	inline void Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg, Tipc_config>::stop() {
		m_is_running = false;
	}
}

#endif // HOST_HPP