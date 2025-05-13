#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "ipc_instance.hpp"
#include "api_request_reader_builder.hpp"

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

inline Instance<ApiRequest> parse_api_request(const RawData& data) {
	throw std::runtime_error("NOT IMPLEMENTED");
}