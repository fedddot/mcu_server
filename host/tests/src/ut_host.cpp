#include <exception>
#include <optional>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "host.hpp"
#include "host_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"
#include "test_vendor.hpp"
#include "vendor_instance.hpp"

using namespace vendor;
using namespace host;
using namespace ipc;

using ManagerId = int;
using Payload = std::string;
using TestHost = Host<ManagerId, Payload>;

TEST(ut_host, ctor_dtor_sanity) {
	// GIVEN
	const auto ipc_data_reader = host::Instance<IpcDataReader<TestHost::ApiRequest>>(
		new TestIpcDataReader<TestHost::ApiRequest>(
			[]()-> std::optional<ipc::Instance<TestHost::ApiRequest>> {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto ipc_data_writer = host::Instance<IpcDataWriter<TestHost::ApiResponse>>(
		new TestIpcDataWriter<TestHost::ApiResponse>(
			[](const TestHost::ApiResponse&){
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto vendor = host::Instance<Vendor<ManagerId, Payload>>(
		new TestVendor<ManagerId, Payload>(
			[](const ApiRequest<ManagerId, Payload>&) -> vendor::Instance<ApiResponse> {
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
			[](const std::exception& e) -> host::Instance<ApiResponse> {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

class TestApiRequest: public ApiRequest<ManagerId, Payload> {
public:
	TestApiRequest(const ManagerId& id, const Payload& payload): m_id(id), m_payload(payload) {}
	TestApiRequest(const TestApiRequest&) = default;
	TestApiRequest& operator=(const TestApiRequest&) = default;
	ManagerId manager_id() const override {
		return m_id;
	}
	vendor::Instance<Payload> payload() const override {
		return vendor::Instance(new Payload(m_payload));
	}
private:
	ManagerId m_id;
	Payload m_payload;
};

class TestApiResponse: public ApiResponse {
public:
	TestApiResponse(const Code& code): m_code(code) {}
	TestApiResponse(const TestApiResponse&) = default;
	TestApiResponse& operator=(const TestApiResponse&) = default;
	
	Code code() const override {
		return m_code;
	}
private:
	Code m_code;
};

TEST(ut_host, run_once_sanity) {
	// GIVEN
	const auto test_api_request = TestApiRequest(10, "test_payload");
	const auto test_api_response = TestApiResponse(ApiResponse::Code::OK);
	const auto ipc_data_reader = host::Instance<IpcDataReader<TestHost::ApiRequest>>(
		new TestIpcDataReader<TestHost::ApiRequest>(
			[test_api_request]()-> std::optional<ipc::Instance<TestHost::ApiRequest>> {
				return ipc::Instance<ApiRequest<ManagerId, Payload>>(new TestApiRequest(test_api_request));
			}
		)
	);
	const auto ipc_data_writer = host::Instance<IpcDataWriter<TestHost::ApiResponse>>(
		new TestIpcDataWriter<TestHost::ApiResponse>(
			[test_api_response](const TestHost::ApiResponse& response){
				ASSERT_EQ(test_api_response.code(), response.code());
			}
		)
	);
	const auto vendor = host::Instance<Vendor<ManagerId, Payload>>(
		new TestVendor<ManagerId, Payload>(
			[test_api_response](const ApiRequest<ManagerId, Payload>&) -> vendor::Instance<ApiResponse> {
				return vendor::Instance<ApiResponse>(new TestApiResponse(test_api_response));
			}
		)
	);

	// WHEN:
	TestHost instance(
		ipc_data_reader,
		ipc_data_writer,
		vendor,
		[](const std::exception& e) -> host::Instance<ApiResponse> {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);

	// THEN:
	ASSERT_NO_THROW(instance.run_once());
}