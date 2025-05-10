#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"
#include "custom_manager.hpp"

using namespace manager;
using namespace host;
using namespace ipc;

using Request = std::string;
using Response = int;

using TestHost = Host<Request, Response>;

TEST(ut_host, ctor_dtor_sanity) {
	// GIVEN
	const auto ipc_data_reader = Instance<IpcDataReader<Request>>(
		new TestIpcDataReader<Request>(
			[]()-> std::optional<Instance<Request>> {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto ipc_data_writer = Instance<IpcDataWriter<Response>>(
		new TestIpcDataWriter<Response>(
			[](const Response&){
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto manager = CustomManager<Request, Response>(
			[](const Request& request) {
				std::cout << "received request: " << request;
				return 0;
			}
		);

	// WHEN:
	TestHost *instance(nullptr);

	// THEN:
	ASSERT_NO_THROW(
		instance = new TestHost(
			ipc_data_reader,
			ipc_data_writer,
			manager,
			[](const std::exception& e) -> Response {
				return Response(-1);
			}
		)
	);
	
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_host, run_once_sanity) {
	// GIVEN
	const auto test_request = Request("test_request");
	const auto expected_response = Response(4);
	const auto ipc_data_reader = Instance<IpcDataReader<Request>>(
		new TestIpcDataReader<Request>(
			[test_request]()-> std::optional<Instance<Request>> {
				return Instance<Request>(new Request(test_request));
			}
		)
	);
	const auto ipc_data_writer = Instance<IpcDataWriter<Response>>(
		new TestIpcDataWriter<Response>(
			[expected_response](const Response& response){
				ASSERT_EQ(expected_response, response);
			}
		)
	);
	const auto manager = CustomManager<Request, Response>(
		[expected_response](const Request& request) {
			std::cout << "received request: " << request << std::endl;
			std::cout << "returning response: " << expected_response << std::endl;
			return expected_response;
		}
	);

	// WHEN:
	TestHost instance(
		ipc_data_reader,
		ipc_data_writer,
		manager,
		[](const std::exception& e) -> Response {
			return Response(-1);
		}	
	);

	// THEN:
	ASSERT_NO_THROW(instance.run_once());
}