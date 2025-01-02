#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "manager.hpp"
#include "provider.hpp"
#include "ipc_connection.hpp"

using namespace mcu_app;

using Request = std::string;
using Response = int;
using ManagerCfg = std::string;
using ProviderCfg = std::string;
using IpcCfg = std::string;

using TestHost = Host<Request, Response, ManagerCfg, ProviderCfg, IpcCfg>;

class TestIpc: public IpcConnection<Request, Response> {
public:
	TestIpc(const IpcCfg& cfg): m_cfg(cfg) {}
	TestIpc(const TestIpc&) = delete;
	TestIpc& operator=(const TestIpc&) = delete;
	bool readable() const override {
		return true;
	}
	Request read() override {
		return Request("test request");
	}
	void write(const Response& data) const override {
		std::cout << "response: " << data << std::endl;
	}
private:
	const IpcCfg m_cfg;
};

class TestProvider: public Provider {
public:
	TestProvider(const ProviderCfg& cfg): m_cfg(cfg) {}
	TestProvider(const TestProvider&) = delete;
	TestProvider& operator=(const TestProvider&) = delete;
	void provide() const {
		std::cout << "provider cfg: " << m_cfg << std::endl;
	}
private:
	const ProviderCfg m_cfg;
};

class TestManager: public Manager<Request, Response> {
public:
	TestManager(Provider *provider, const ManagerCfg& cfg): m_provider(dynamic_cast<TestProvider *>(provider)) {
		if (!m_provider) {
			throw std::invalid_argument("invalid provider received");
		}
	}
	TestManager(const TestManager&) = delete;
	TestManager& operator=(const TestManager&) = delete;
	
	Response run(const Request& request) override {
		m_provider->provide();
		return Response(0);
	}
private:
	TestProvider *m_provider;
};

TEST(ut_host, sanity) {
	// GIVEN
	const auto ipc_cfg = IpcCfg("ipc config");
	auto ipc_factory = [](const IpcCfg& cfg) {
		return new TestIpc(cfg);
	};
	const auto manager_cfg = ManagerCfg("manager config");
	auto manager_factory = [](Provider *provider, const ManagerCfg& cfg) {
		return new TestManager(provider, cfg);
	};
	const auto provider_cfg = ProviderCfg("provider config");
	auto provider_factory = [](const ProviderCfg& cfg) {
		return new TestProvider(cfg);
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
			provider_factory,
			provider_cfg,
			[](const std::exception& e) {
				return -1;
			}
		)
	);
	ASSERT_NO_THROW(instance->run_once());
	ASSERT_NO_THROW(delete instance);
}