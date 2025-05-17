#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"
#include "vendor_instance.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"

using namespace vendor;
using namespace host;
using namespace ipc;

using ManagerId = int;
using Payload = std::string;
using TestHost = Host<ManagerId, Payload>;

TEST(ut_host, ctor_dtor_sanity) {
	// GIVEN
	const auto ipc_data_reader = ipc::Instance<IpcDataReader<TestHost::ApiRequest>>(
		new TestIpcDataReader<TestHost::ApiRequest>(
			[]()-> std::optional<ipc::Instance<TestHost::ApiRequest>> {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto ipc_data_writer = ipc::Instance<IpcDataWriter<TestHost::ApiResponse>>(
		new TestIpcDataWriter<TestHost::ApiResponse>(
			[](const TestHost::ApiResponse&){
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	// const auto vendor = vendor::Instance<Vendor<ManagerId, Payload>>(
	// 	new CustomVendor<MovementVendorRequest, MovementVendorResponse>(
	// 		[](const ManagerId& request) {
	// 			std::cout << "received request: " << request;
	// 			return 0;
	// 		}
	// 	)
	// );

	// WHEN:
	TestHost *instance(nullptr);

	// THEN:
	// ASSERT_NO_THROW(
	// 	instance = new TestHost(
	// 		ipc_data_reader,
	// 		ipc_data_writer,
	// 		vendor,
	// 		[](const std::exception& e) -> Response {
	// 			return Response(-1);
	// 		}
	// 	)
	// );
	
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

// TEST(ut_host, run_once_sanity) {
// 	// GIVEN
// 	const auto test_request = ManagerId("test_request");
// 	const auto expected_response = Payload(4);
// 	const auto ipc_data_reader = ipc::Instance<IpcDataReader<ManagerId>>(
// 		new TestIpcDataReader<ManagerId>(
// 			[test_request]()-> std::optional<ipc::Instance<ManagerId>> {
// 				return ipc::Instance<ManagerId>(new ManagerId(test_request));
// 			}
// 		)
// 	);
// 	const auto ipc_data_writer = ipc::Instance<IpcDataWriter<Payload>>(
// 		new TestIpcDataWriter<Payload>(
// 			[expected_response](const Payload& response){
// 				ASSERT_EQ(expected_response, response);
// 			}
// 		)
// 	);
// 	const auto vendor = vendor::Instance<Vendor<ManagerId, Payload>>(
// 		new CustomVendor<ManagerId, Payload>(
// 			[expected_response](const ManagerId& request) {
// 				std::cout << "received request: " << request << std::endl;
// 				std::cout << "returning response: " << expected_response << std::endl;
// 				return expected_response;
// 			}
// 		)
// 	);

// 	// WHEN:
// 	TestHost instance(
// 		ipc_data_reader,
// 		ipc_data_writer,
// 		vendor,
// 		[](const std::exception& e) -> Payload {
// 			return Payload(-1);
// 		}	
// 	);

// 	// THEN:
// 	ASSERT_NO_THROW(instance.run_once());
// }