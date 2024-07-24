#include <map>
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "array.hpp"
#include "data.hpp"
#include "decoding_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "string.hpp"

using namespace engine;
using namespace mcu_server_utl;

const std::map<std::string, std::string> s_conversion_map {
	{"key0", "0"},
	{"key1", "1"}
};

TEST(ut_decoding_data_parser, parse_sanity) {
	// GIVEN:
	Object test_encoded_data_inner_object;
	test_encoded_data_inner_object.add("0", String("string data"));
	test_encoded_data_inner_object.add("1", String("another string data"));
	Array test_encoded_data_inner_array;
	test_encoded_data_inner_array.push_back(String("array string"));
	test_encoded_data_inner_array.push_back(test_encoded_data_inner_object);
	Object test_encoded_data_object;
	test_encoded_data_object.add("0", String("string data"));
	test_encoded_data_object.add("1", test_encoded_data_inner_array);
	
	const std::string test_serial_data(JsonDataSerializer().serialize(test_encoded_data_object));

	// WHEN:
	DecodingDataParser instance(s_conversion_map);
	std::unique_ptr<Data> result(nullptr);

	// THEN:
	ASSERT_NO_THROW(result = std::unique_ptr<Data>(instance.parse(test_serial_data)));
	ASSERT_NE(nullptr, result);
	Object *result_obj(dynamic_cast<Object *>(result.get()));
	ASSERT_NE(nullptr, result_obj);
	ASSERT_TRUE(result_obj->contains("key0"));
	ASSERT_TRUE(result_obj->contains("key1"));
}