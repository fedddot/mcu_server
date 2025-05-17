#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "json/config.h"
#include "json/value.h"
#include "json/writer.h"

#include "host.hpp"
#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "ipc_instance.hpp"
#include "json_ipc_data_reader.hpp"
#include "json_ipc_data_writer.hpp"
#include "manager_instance.hpp"
#include "raw_data_package_descriptor.hpp"
#include "raw_data_package_reader.hpp"
#include "raw_data_package_utils.hpp"
#include "raw_data_package_writer.hpp"
#include "custom_manager.hpp"
#include "test_ipc_data_writer.hpp"

using namespace manager;
using namespace host;
using namespace ipc;

using ManagerId = std::string;
using Payload = int;

using TestHost = Host<ManagerId, Payload>;

static ipc::Instance<IpcDataReader<ManagerId>> create_ipc_reader(RawData *buffer, const RawDataPackageDescriptor& desc);
static ipc::Instance<IpcDataWriter<Payload>> create_ipc_writer(const RawDataPackageDescriptor& desc, const ipc::Instance<IpcDataWriter<RawData>>& raw_data_writer);

static RawData generate_serial_request(const RawDataPackageDescriptor& desc, const ManagerId& request);

TEST(ut_host_raw_data_packages, run_once_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = RawData(preamble_str.begin(), preamble_str.end());
	const auto package_descriptor = RawDataPackageDescriptor(
		preamble,
		4UL
	);

	const auto test_request = ManagerId("test_request");
	const auto expected_response = Payload(4);
	const auto junk_before_str = std::string("junk");
	const auto junk_before = RawData(junk_before_str.begin(), junk_before_str.end());
	const auto raw_data_writer = ipc::Instance<IpcDataWriter<RawData>>(
		new TestIpcDataWriter<RawData>(
			[expected_response, package_descriptor](const RawData& response_raw_data) {
				const auto received_preamble = RawData(
					response_raw_data.begin(),
					response_raw_data.begin() + package_descriptor.preamble().size()
				);
				ASSERT_EQ(package_descriptor.preamble(), received_preamble);
			}
		)
	);
	// WHEN
	auto buff = RawData();
	
	auto ipc_reader = create_ipc_reader(&buff, package_descriptor);
	auto ipc_writer = create_ipc_writer(
		package_descriptor,
		raw_data_writer
	);
	const auto manager = manager::Instance<Manager<ManagerId, Payload>>(
		new CustomManager<ManagerId, Payload> (
			[expected_response, test_request](const ManagerId& request) {
				std::cout << "test manager received request: " << request << std::endl;
				std::cout << "test manager is sending response: " << expected_response << std::endl;
				if (test_request != request) {
					throw std::runtime_error("received unexpected request");
				}
				return expected_response;
			}
		)
	);
	TestHost instance(
		ipc_reader,
		ipc_writer,
		manager,
		[](const std::exception&) -> Payload {
			return -1;
		}
	);
	const auto test_serial_request = generate_serial_request(package_descriptor, test_request);

	// THEN:
	// empty buffer
	ASSERT_NO_THROW(instance.run_once());
	
	// junk data should be ignored
	buff.insert(
		buff.end(),
		junk_before.begin(),
		junk_before.end()
	);
	ASSERT_NO_THROW(instance.run_once());

	buff.insert(
		buff.end(),
		test_serial_request.begin(),
		test_serial_request.end()
	);
	ASSERT_NO_THROW(instance.run_once());
	
	ASSERT_TRUE(buff.empty());
}

inline ipc::Instance<IpcDataReader<ManagerId>> create_ipc_reader(RawData *buffer, const RawDataPackageDescriptor& desc) {
	const auto ipc_data_reader = ipc::Instance<IpcDataReader<RawData>>(new RawDataPackageReader(
		buffer,
		desc,
		parse_package_size
	));
	auto request_retriever = [](const Json::Value& json_data) -> ipc::Instance<ManagerId> {
		const auto request_data = json_data["request"].asString();
		return ipc::Instance<ManagerId>(new ManagerId(request_data));
	};
	return ipc::Instance<IpcDataReader<ManagerId>>(
		new JsonIpcDataReader<ManagerId>(
			ipc_data_reader,
			request_retriever
		)
	);
}

inline ipc::Instance<IpcDataWriter<Payload>> create_ipc_writer(const RawDataPackageDescriptor& desc, const ipc::Instance<IpcDataWriter<RawData>>& raw_data_writer) {
	const auto ipc_data_writer = ipc::Instance<IpcDataWriter<RawData>>(
		new RawDataPackageWriter(
			desc,
			serialize_package_size,
			raw_data_writer
		)
	);
	auto response_serializer = [](const Payload& resp) -> Json::Value {
		return Json::Value(resp);
	};
	return ipc::Instance<IpcDataWriter<Payload>>(
		new JsonIpcDataWriter<Payload>(
			ipc_data_writer,
			response_serializer
		)
	);
}

inline RawData generate_serial_request(const RawDataPackageDescriptor& desc, const ManagerId& request) {
	auto request_json_data = Json::Value();
	request_json_data["request"] = Json::String(request);
    const auto request_serial_data = Json::writeString(
		Json::StreamWriterBuilder(),
		request_json_data
	);
	const auto preamble = desc.preamble();
	const auto size_data = serialize_package_size(desc, request_serial_data.size());
	auto payload = RawData();
	payload.insert(
		payload.end(),
		preamble.begin(),
		preamble.end()
	);
	payload.insert(
		payload.end(),
		size_data.begin(),
		size_data.end()
	);
	payload.insert(
		payload.end(),
		request_serial_data.begin(),
		request_serial_data.end()
	);
    return payload;
}