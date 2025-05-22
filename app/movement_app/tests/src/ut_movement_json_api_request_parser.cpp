#include <string>

#include "gtest/gtest.h"

#include "movement_json_api_request_parser.hpp"
#include "movement_vendor_api_request.hpp"

using namespace ipc;

using AxesConfig = std::string;

TEST(ut_movement_json_api_request_parser, ctor_dtor_sanity) {
	// WHEN
	MovementJsonApiRequestParser<AxesConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new MovementJsonApiRequestParser<AxesConfig>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_json_api_request_parser, parse) {
	// GIVEN
	Json::Value json_value;
	json_value["request_type"] = "CONFIG";
	
	// WHEN
	MovementJsonApiRequestParser<AxesConfig> instance;
	
	// THEN
	try {
		const auto result = instance.parse(json_value);
	} catch (const std::exception& e) {
		FAIL() << "Exception thrown: " << e.what();
	}
}