#include <string>

#include "gtest/gtest.h"

#include "json_data_serializer.hpp"
#include "json_request_parser.hpp"
#include "object.hpp"
#include "request.hpp"
#include "server_types.hpp"
#include "string.hpp"

using namespace server;
using namespace server_utl;

TEST(ut_json_request_parser, parse_sanity) {
	// GIVEN:
	Object test_data;
	test_data.add("method", String("CREATE"));
	test_data.add("path", String("one/two/three"));
	test_data.add("body", Object());
	auto test_data_serial(JsonDataSerializer()(test_data));

	// WHEN:
	JsonRequestParser instance;
	Request result(Request::Method::READ, {}, Body());

	// THEN:
	ASSERT_NO_THROW(result = instance(test_data_serial));
	ASSERT_EQ(Request::Method::CREATE, result.method());
	ASSERT_EQ(Path({"one", "two", "three"}), result.path());

}