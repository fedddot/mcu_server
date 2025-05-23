#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"
#include "api_request_reader_builder.hpp"
#include "test_ipc_data_reader.hpp"

using namespace ipc;

using ApiRequest = std::string;
using RawData = std::vector<char>;

static Instance<ApiRequest> parse_api_request(const RawData& data);

TEST(ut_api_request_reader_builder, ctor_dtor_sanity) {
	// GIVEN
	
	// WHEN
	ApiRequestReaderBuilder<ApiRequest, RawData> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new ApiRequestReaderBuilder<ApiRequest, RawData>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}


TEST(ut_api_request_reader_builder, build_sanity) {
	// GIVEN
	const auto test_reader = Instance<IpcDataReader<RawData>>(
		new TestIpcDataReader<RawData>(
			[]() -> std::optional<Instance<RawData>> {
				return std::nullopt;
			}
		)
	);
	// WHEN
	ApiRequestReaderBuilder<ApiRequest, RawData> instance;
	instance.set_raw_data_reader(test_reader);
	instance.set_api_request_parser(parse_api_request);

	// THEN
	ASSERT_NO_THROW(
		{
			const auto reader_instance = instance.build();
		}
	);
}

inline Instance<ApiRequest> parse_api_request(const RawData& data) {
	throw std::runtime_error("NOT IMPLEMENTED");
}