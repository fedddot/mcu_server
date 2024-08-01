#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "json_data_parser.hpp"
#include "object.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;

TEST(ut_buffered_custom_ipc_connection, parse_sanity) {
	// GIVEN:
	const std::string test_data("{\"key1\": 1, \"key2\": \"ahahaha\", \"key3\": {\"key4\": \"ahaha\"}, \"key6\": [0, 1, 2, {}]}");

	// WHEN:
	JsonDataParser instance;
	std::unique_ptr<Data> result(nullptr);

	// THEN:
	ASSERT_NO_THROW(result = std::unique_ptr<Data>(instance.parse(test_data)));
	ASSERT_NE(nullptr, result);
	Object *result_obj(dynamic_cast<Object *>(result.get()));
	ASSERT_NE(nullptr, result_obj);
	ASSERT_TRUE(result_obj->contains("key1"));
	ASSERT_TRUE(result_obj->contains("key2"));
	ASSERT_TRUE(result_obj->contains("key3"));
}