#include <cstddef>
#include <exception>
#include <optional>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "host.hpp"
#include "sized_package_infra.hpp"
#include "raw_data_package_reader.hpp"
#include "raw_data_package_writer.hpp"
#include "test_manager.hpp"

using namespace manager;
using namespace manager_tests;
using namespace host;
using namespace ipc;

using Request = std::vector<char>;
using Response = std::vector<char>;

using TestHost = Host<Request, Response>;

TEST(ut_host_sized_packages, run_once_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = std::vector<char>(preamble_str.begin(), preamble_str.end());
	const auto expected_response_str = std::string("expected_response");
	const auto expected_response = std::vector<char>(expected_response_str.begin(), expected_response_str.end());
	const auto test_request = Request {'a', 'b', 'c'};
	const auto test_request_size_encoded = SizedPackageInfra::encode_size(test_request.size());
	const auto junk_before_str = std::string("junk");
	const auto junk_before = std::vector<char>(junk_before_str.begin(), junk_before_str.end());
	
	// WHEN
	auto buff = std::vector<char>();
	auto response_data_option = std::optional<std::vector<char>>();
	auto response_data_option_ptr = &response_data_option;
	auto ipc_data_reader = SizedPackageReader(
		&buff,
		preamble
	);
	auto ipc_data_writer = SizedPackageWriter(
		[response_data_option_ptr](const std::vector<char>& response_raw) {
			*response_data_option_ptr = response_raw;
		},
		preamble
	);
	auto manager = TestManager<Request, Response> (
		[expected_response](const Request& request) {
			return expected_response;
		}
	);
	TestHost instance(
		&ipc_data_reader,
		&ipc_data_writer,
		&manager,
		[](const std::exception& e) -> Response {
			const auto e_str = std::string(e.what());
			return Response(e_str.begin(), e_str.end());
		}
	);

	// THEN:
	// empty buffer
	ASSERT_NO_THROW(instance.run_once());
	ASSERT_FALSE(response_data_option);
	
	// junk data should be ignored
	buff.insert(
		buff.end(),
		junk_before.begin(),
		junk_before.end()
	);
	ASSERT_NO_THROW(instance.run_once());
	ASSERT_FALSE(response_data_option);

	buff.insert(
		buff.end(),
		preamble.begin(),
		preamble.end()
	);
	ASSERT_NO_THROW(instance.run_once());
	ASSERT_FALSE(response_data_option);
	
	buff.insert(
		buff.end(),
		test_request_size_encoded.begin(),
		test_request_size_encoded.end()
	);
	ASSERT_NO_THROW(instance.run_once());
	ASSERT_FALSE(response_data_option);
	
	buff.insert(
		buff.end(),
		test_request.begin(),
		test_request.end()
	);
	ASSERT_NO_THROW(instance.run_once());
	ASSERT_TRUE(response_data_option);
	const auto decoded_response = Response(
		response_data_option->begin() + preamble.size() + sizeof(std::size_t),
		response_data_option->end()
	);
	ASSERT_EQ(expected_response, decoded_response);

	ASSERT_TRUE(buff.empty());
}