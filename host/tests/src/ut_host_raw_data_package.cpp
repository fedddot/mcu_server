#include "json/value.h"
#include <cstddef>
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "host.hpp"
#include "json_ipc_data_writer.hpp"
#include "raw_data_package_reader.hpp"
#include "raw_data_package_writer.hpp"
#include "raw_data_package_descriptor.hpp"
#include "raw_data_package_utils.hpp"
#include "json_ipc_data_reader.hpp"
#include "shared_ipc_data_reader.hpp"
#include "shared_ipc_data_writer.hpp"
#include "test_manager.hpp"

using namespace manager;
using namespace manager_tests;
using namespace host;
using namespace ipc;

using Request = std::string;
using Response = int;
using RawData = typename RawDataPackageDescriptor::RawData;

using TestHost = Host<Request, Response>;

static SharedIpcDataReader<Request> create_ipc_reader(RawData *buffer, const RawDataPackageDescriptor& desc);
static SharedIpcDataWriter<Response> create_ipc_writer(const RawDataPackageDescriptor& desc, const RawDataPackageWriter::RawDataWriter& raw_data_writer);

TEST(ut_host_raw_data_packages, run_once_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = RawData(preamble_str.begin(), preamble_str.end());
	const auto package_descriptor = RawDataPackageDescriptor(
		preamble,
		4UL
	);

	const auto test_request = Request("test_request");
	const auto expected_response = Response(4);
	const auto junk_before_str = std::string("junk");
	const auto junk_before = RawData(junk_before_str.begin(), junk_before_str.end());
	
	// WHEN
	auto buff = RawData();
	
	auto ipc_reader = create_ipc_reader(&buff, package_descriptor);
	auto ipc_writer = create_ipc_writer(
		package_descriptor,
		[expected_response](const RawData& response_raw_data) {
			
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	const auto manager = TestManager<Request, Response> (
		[expected_response, test_request](const Request& request) {
			std::cout << "test manager received request: " << request << std::endl;
			std::cout << "test manager is sending response: " << expected_response << std::endl;
			if (test_request != request) {
				throw std::runtime_error("received unexpected request");
			}
			return expected_response;
		}
	);
	TestHost instance(
		ipc_reader,
		ipc_writer,
		manager,
		[](const std::exception&) -> Response {
			return -1;
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

inline SharedIpcDataReader<Request> create_ipc_reader(RawData *buffer, const RawDataPackageDescriptor& desc) {
	const auto ipc_data_reader = RawDataPackageReader(
		buffer,
		desc,
		parse_package_size
	);
	auto request_retriever = [](const Json::Value& json_data) -> Request {
		const auto request_data = json_data["request"].asString();
		return Request(request_data);
	};
	return SharedIpcDataReader<Request>(
		new JsonIpcDataReader<Request>(
			ipc_data_reader,
			request_retriever
		)
	);
}

inline SharedIpcDataWriter<Response> create_ipc_writer(const RawDataPackageDescriptor& desc, const RawDataPackageWriter::RawDataWriter& raw_data_writer) {
	const auto ipc_data_writer = RawDataPackageWriter(
		desc,
		serialize_package_size,
		raw_data_writer
	);
	auto response_serializer = [](const Response& resp) -> Json::Value {
		return Json::Value(resp);
	};
	return SharedIpcDataWriter<Response>(
		new JsonIpcDataWriter<Response>(
			ipc_data_writer,
			response_serializer
		)
	);
}