#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "test_request_reader.hpp"
#include "test_response_writer.hpp"
#include "test_manager.hpp"

using namespace manager;
using namespace manager_tests;
using namespace host;
using namespace ipc;

using Request = std::string;
using Response = int;

using TestHost = Host<Request, Response>;

TEST(ut_host, ctor_dtor_sanity) {
	// GIVEN
	auto request_reader = TestRequestReader<Request>(
		[]()-> std::optional<Request> {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	auto response_writer = TestResponseWriter<Response>(
		[](const Response&){
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	auto manager = TestManager<Request, Response>(
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
			&request_reader,
			&response_writer,
			&manager,
			[](const std::exception& e) -> Response {
				return Response(-1);
			}
		)
	);
	
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}