#include "gtest/gtest.h"
#include <cstdint>

#include "proto_thermostat_api_response_serializer.hpp"
#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"

using namespace ipc;
using namespace service;
using RawData = std::vector<std::uint8_t>;

static RawData serialize_thermostat_request(const ThermostatApiRequest& request);

TEST(ut_api_response_serializer, sanity) {
	// GIVEN
	const auto response = ThermostatServiceApiResponse(
		ThermostatServiceApiResponse::Result::SUCCESS,
		"Operation completed successfully",
		25.0
	);
	
	// WHEN
	const auto instance = ApiResponseSerializer();

	// THEN
	ASSERT_NO_THROW({
		const auto result = instance(response);
		ASSERT_FALSE(result.empty());
	});
}