#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "custom_data_reader.hpp"
#include "custom_data_writer.hpp"
#include "manager.hpp"
#include "provider.hpp"

using namespace app;
using namespace ipc;
using namespace provider;
using namespace manager;

using Request = std::string;
using Response = std::string;
using ManagerCfg = std::string;
using ProviderCfg = std::string;

using TestHost = Host<Request, Response, ManagerCfg, ProviderCfg>;

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
	TestManager(Provider *provider): m_provider(dynamic_cast<TestProvider *>(provider)) {
		if (!m_provider) {
			throw std::invalid_argument("invalid provider received");
		}
	}
	TestManager(const TestManager&) = delete;
	TestManager& operator=(const TestManager&) = delete;
	
	Response run(const Request& request) override {
		m_provider->provide();
		return Response("response");
	}
private:
	TestProvider *m_provider;
};

TEST(ut_host, sanity) {
	// GIVEN
	CustomDataReader<Request> reader(
		[](void) {
			return Request("request");
		},
		[](void) {
			return true;
		}
	);
	CustomDataWriter<Response> writer(
		[](const Response& response) {
			std::cout << "response: " << response << std::endl;
		}
	);
	// WHEN:
	TestHost *instance(nullptr);

	// THEN:
	ASSERT_NO_THROW(
		instance = new TestHost(
			&reader,
			&writer,
			[](const std::exception& e)-> Response {
				throw std::runtime_error("NOT IMPLEMENTED");
			},
			ManagerCfg("manager_cfg"),
			[](provider::Provider *provider, const ManagerCfg&)-> manager::Manager<Request, Response> * {
				return new TestManager(provider);
			},
			ProviderCfg("provider_cfg"),
			[](const ProviderCfg& cfg)-> provider::Provider * {
				return new TestProvider(cfg);
			}
		)
	);
	ASSERT_NO_THROW(instance->run_once());
	ASSERT_NO_THROW(delete instance);
}