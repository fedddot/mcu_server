#include <exception>
#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "ipc_config.hpp"
#include "test_ipc_server.hpp"
#include "test_manager.hpp"

using namespace manager;
using namespace manager_tests;
using namespace host;
using namespace ipc_tests;
using namespace ipc;

using Request = std::string;
using Response = int;
using ManagerCfg = std::string;

using TestHost = Host<Request, Response, ManagerCfg>;

TEST(ut_host, sanity) {
	// GIVEN
	const auto ipc_cfg = TestIpcConfig<Response>(
		[](const Response& response) {

		},
		1000
	);
	auto ipc_factory = [](const IpcConfig& cfg) {
		const auto& cfg_casted = dynamic_cast<const TestIpcConfig<Response>&>(cfg);
		return new TestIpcServer<Request, Response>(cfg_casted);
	};
	const auto manager_cfg = ManagerCfg("manager config");
	auto manager_factory = [](const ManagerCfg& cfg) {
		return new TestManager<Request, Response>(
			[](const Request& request) {
				std::cout << "received request: " << request;
				return 0;
			}
		);
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
			[](const std::exception& e) {
				return -1;
			}
		)
	);
	
	ASSERT_NO_THROW(instance->run_once());
	ASSERT_NO_THROW(delete instance);
}