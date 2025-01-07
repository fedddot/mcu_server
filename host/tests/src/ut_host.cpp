#include <exception>
#include <functional>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "manager.hpp"
#include "providers.hpp"
#include "test_ipc_server.hpp"
#include "test_providers.hpp"

using namespace manager;
using namespace host;
using namespace host_tests;
using namespace ipc;

using Request = std::string;
using Response = int;
using ProviderId = std::string;
using ProvidersCfg = std::string;
using ManagerCfg = std::string;
using IpcCfg = std::string;

using TestHost = Host<Request, Response, ProviderId, ProvidersCfg, ManagerCfg, IpcCfg>;

class TestManager: public Manager<Request, Response> {
public:
	TestManager(Providers<ProviderId> *provider, const ManagerCfg& cfg): m_provider(provider) {
		if (!m_provider) {
			throw std::invalid_argument("invalid provider received");
		}
	}
	TestManager(const TestManager&) = delete;
	TestManager& operator=(const TestManager&) = delete;
	
	Response run(const Request& request) override {
		(void)request;
		return Response(0);
	}
private:
	Providers<ProviderId> *m_provider;
};

TEST(ut_host, sanity) {
	// GIVEN
	const auto ipc_cfg = IpcCfg("ipc config");
	auto ipc_factory = [](const IpcCfg& cfg) {
		return new TestIpcServer<Request, Response>(
			[](const Response& response) {

			},
			1000
		);
	};
	const auto manager_cfg = ManagerCfg("manager config");
	auto manager_factory = [](Providers<ProviderId> *providers, const ManagerCfg& cfg) {
		return new TestManager(providers, cfg);
	};
	const auto providers_cfg = ProvidersCfg("providers config");
	auto providers_factory = [](const ProvidersCfg& cfg) {
		return new TestProviders<ProviderId>();
	};
	// WHEN:
	TestHost *instance(nullptr);

	// THEN:
	ASSERT_NO_THROW(
		instance = new TestHost(
			ipc_factory,
			ipc_cfg,
			manager_factory,
			manager_cfg,
			providers_factory,
			providers_cfg,
			[](const std::exception& e) {
				return -1;
			}
		)
	);
	
	ASSERT_NO_THROW(instance->run_once());
	ASSERT_NO_THROW(delete instance);
}