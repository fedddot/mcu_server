#include <exception>
#include <optional>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "ipc_instance.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"
#include "test_vendor.hpp"

using namespace vendor;
using namespace host;
using namespace ipc;

using ApiRequest = std::string;
using ApiResponse = int;
using TestHost = Host<ApiRequest, ApiResponse>;

TEST(ut_host, ctor_dtor_sanity) {
	// GIVEN
	const auto ipc_data_reader = TestHost::ApiRequestReaderInstance(
		new TestIpcDataReader<ApiRequest>(
			[]()-> std::optional<ipc::Instance<ApiRequest>> {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto ipc_data_writer = TestHost::ApiResponseWriterInstance(
		new TestIpcDataWriter<ApiResponse>(
			[](const ApiResponse&){
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto vendor = TestHost::VendorInstance(
		new TestVendor<ApiRequest, ApiResponse>(
			[](const ApiRequest&) -> ApiResponse {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);

	// WHEN:
	TestHost *instance(nullptr);

	THEN:
	ASSERT_NO_THROW(
		instance = new TestHost(
			ipc_data_reader,
			ipc_data_writer,
			vendor,
			[](const std::exception& e) -> ApiResponse {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_host, run_once_sanity) {
	// GIVEN
	const auto test_api_request = ApiRequest("test_request");
	const auto test_api_response = ApiResponse(12);
	const auto ipc_data_reader = TestHost::ApiRequestReaderInstance(
		new TestIpcDataReader<ApiRequest>(
			[test_api_request]()-> std::optional<ipc::Instance<ApiRequest>> {
				return ipc::Instance<ApiRequest>(new ApiRequest(test_api_request));
			}
		)
	);
	const auto ipc_data_writer = TestHost::ApiResponseWriterInstance(
		new TestIpcDataWriter<ApiResponse>(
			[test_api_response](const ApiResponse& response){
				ASSERT_EQ(test_api_response, response);
			}
		)
	);
	const auto vendor = TestHost::VendorInstance(
		new TestVendor<ApiRequest, ApiResponse>(
			[test_api_response](const ApiRequest&) -> ApiResponse {
				return ApiResponse(test_api_response);
			}
		)
	);

	// WHEN:
	TestHost instance(
		ipc_data_reader,
		ipc_data_writer,
		vendor,
		[](const std::exception& e) -> ApiResponse {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);

	// THEN:
	ASSERT_NO_THROW(instance.run_once());
}