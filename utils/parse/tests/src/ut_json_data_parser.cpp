#include <string>

#include "gtest/gtest.h"

#include "json_data_parser.hpp"
#include "object.hpp"

using namespace server;
using namespace server_utl;

TEST(ut_json_data_parser, parse_sanity) {
	// GIVEN:
	const std::string test_data("{\"key1\": 1, \"key2\": \"ahahaha\", \"key3\": {\"key4\": \"ahaha\"}");

	// WHEN:
	JsonDataParser instance;
	Object result;

	// THEN:
	ASSERT_NO_THROW(result = instance.parse(test_data));
}