#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

#include "data_reader.hpp"
#include "data_writer.hpp"
#include "manager.hpp"
#include "provider.hpp"

namespace app {
	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg>
	class Host {
	public:
		using RequestReader = ipc::DataReader<Trequest>;
		using ResponseWriter = ipc::DataWriter<Tresponse>;
		using FailureResponseGenerator = std::function<Tresponse(const std::exception&)>;
		using ManagerFactory = std::function<manager::Manager<Trequest, Tresponse> *(provider::Provider *, const Tmanager_cfg&)>;
		using ProviderFactory = std::function<provider::Provider *(const Tprovider_cfg&)>;
		Host(
			RequestReader *request_reader,
			ResponseWriter *response_writer,
			const FailureResponseGenerator& failure_response_generator,
			const Tmanager_cfg& manager_config,
			const ManagerFactory& manager_factory,
			const Tprovider_cfg& provider_config,
			const ProviderFactory& provider_factory
		);
		virtual ~Host() noexcept = default;
		void run_once();
		void run();
		bool is_running() const;
		void stop();
	private:
		RequestReader *m_request_reader;
		ResponseWriter *m_response_writer;
		FailureResponseGenerator m_failure_response_generator;
		bool m_is_running;

		std::unique_ptr<provider::Provider> m_provider;
		std::unique_ptr<manager::Manager<Trequest, Tresponse>> m_manager;
	};

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg>
	inline Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg>::Host(
		RequestReader *request_reader,
		ResponseWriter *response_writer,
		const FailureResponseGenerator& failure_response_generator,
		const Tmanager_cfg& manager_config,
		const ManagerFactory& manager_factory,
		const Tprovider_cfg& provider_config,
		const ProviderFactory& provider_factory
	): m_request_reader(request_reader), m_response_writer(response_writer), m_failure_response_generator(failure_response_generator), m_is_running(false) {
		using namespace manager;
		using namespace provider;
		if (!request_reader || !response_writer || !manager_factory || !provider_factory || !failure_response_generator) {
			throw std::invalid_argument("invalid host args");
		}
		m_provider = std::unique_ptr<Provider>(provider_factory(provider_config));
		m_manager = std::unique_ptr<Manager<Trequest, Tresponse>>(manager_factory(m_provider.get(), manager_config));
	}

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg>
	inline void Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg>::run_once() {
		try {
			if (!m_request_reader->readable()) {
				return;
			}
			const auto request = m_request_reader->read();
			const auto response = m_manager->run(request);
			m_response_writer->write(response);
		} catch (const std::exception& e) {
			m_response_writer->write(m_failure_response_generator(e));
		}
	}

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg>
	inline void Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg>::run() {
		m_is_running = true;
		while (m_is_running) {
			run_once();
		}
	}

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg>
	inline bool Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg>::is_running() const {
		return m_is_running;
	}

	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tprovider_cfg>
	inline void Host<Trequest, Tresponse, Tmanager_cfg, Tprovider_cfg>::stop() {
		m_is_running = false;
	}
}

#endif // HOST_HPP