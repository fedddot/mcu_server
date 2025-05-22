#include <string>

#include "gtest/gtest.h"

#include "axes_controller_config_request.hpp"
#include "movement_json_api_request_parser.hpp"
#include "movement_vendor_api_request.hpp"

using namespace ipc;
using namespace vendor;

using AxesConfig = std::string;

TEST(ut_movement_json_api_request_parser, ctor_dtor_sanity) {
	// WHEN
	MovementJsonApiRequestParser<AxesConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new MovementJsonApiRequestParser<AxesConfig>(
				[](const Json::Value&) -> AxesConfig {
					return "axes_config";
				}
			)
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_json_api_request_parser, parse_config_request) {
	// GIVEN
	const auto expected_request_type = MovementVendorApiRequest::RequestType::CONFIG;
	const auto expected_axes_config = "test_axes_config";
	Json::Value json_value;
	json_value["request_type"] = "CONFIG";
	json_value["axes_config"] = expected_axes_config;
	
	// WHEN
	MovementJsonApiRequestParser<AxesConfig> instance(
		[](const Json::Value& json_request) -> AxesConfig {
			return json_request["axes_config"].asString();
		}
	);
	
	// THEN
	ASSERT_NO_THROW({
		const auto request = instance.parse(json_value);
		ASSERT_EQ(expected_request_type, request.get().type());
		const auto& casted_request = dynamic_cast<const AxesControllerConfigApiRequest<AxesConfig>&>(request.get());
		ASSERT_EQ(expected_axes_config, casted_request.axes_cfg());
	});
}