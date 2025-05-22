#include <string>

#include "gtest/gtest.h"

#include "movement_json_api_request_parser.hpp"

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