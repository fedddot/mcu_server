#include "gtest/gtest.h"
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "array.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "string.hpp"

using namespace manager;

TEST(ut_string, sanity) {
	// GIVEN
	std::string test_str1("test_str1");
	std::string test_str2("test_str2");

	// WHEN
	std::unique_ptr<String> test_instance_ptr1(nullptr);
	std::unique_ptr<String> test_instance_ptr2(nullptr);

	// THEN
	ASSERT_NO_THROW(test_instance_ptr1 = std::unique_ptr<String>(new String(test_str1)));
	ASSERT_NE(nullptr, test_instance_ptr1);
	ASSERT_EQ(test_str1, test_instance_ptr1->get());
	ASSERT_NO_THROW(test_instance_ptr2 = std::unique_ptr<String>(new String(*test_instance_ptr1)));
	ASSERT_EQ(test_str1, test_instance_ptr2->get());
}

TEST(ut_object, sanity) {
	// GIVEN
	const std::map<std::string, std::shared_ptr<Data>> test_data {
		{"int_val", std::shared_ptr<Data>(new Integer(10))},
		{"str_val", std::shared_ptr<Data>(new String("str"))}
	};

	// WHEN
	std::unique_ptr<Object> test_instance_ptr1(nullptr);
	std::unique_ptr<Object> test_instance_ptr2(nullptr);

	// THEN
	ASSERT_NO_THROW(test_instance_ptr1 = std::unique_ptr<Object>(new Object()));

	std::for_each(
		test_data.begin(),
		test_data.end(),
		[&](auto& iter) {
			ASSERT_NO_THROW(test_instance_ptr1->add(iter.first, *iter.second));
		}
	);

	ASSERT_EQ(test_data.size(), test_instance_ptr1->size());

	std::for_each(
		test_data.begin(),
		test_data.end(),
		[&](auto& iter) {
			ASSERT_TRUE(test_instance_ptr1->contains(iter.first));
		}
	);

	ASSERT_NO_THROW(test_instance_ptr2 = std::unique_ptr<Object>(new Object(*test_instance_ptr1)));
	ASSERT_EQ(test_instance_ptr1->size(), test_instance_ptr2->size());
	ASSERT_NO_THROW(test_instance_ptr2->remove(test_data.begin()->first));
	ASSERT_EQ(test_instance_ptr1->size(), test_instance_ptr2->size() + 1);
}

TEST(ut_array, sanity) {
	// GIVEN
	const std::vector<std::shared_ptr<Data>> test_data {
		std::shared_ptr<Data>(new Integer(10)),
		std::shared_ptr<Data>(new String("str"))
	};

	// WHEN
	std::unique_ptr<Array> test_instance_ptr1(nullptr);
	std::unique_ptr<Array> test_instance_ptr2(nullptr);

	// THEN
	ASSERT_NO_THROW(test_instance_ptr1 = std::unique_ptr<Array>(new Array()));

	std::for_each(
		test_data.begin(),
		test_data.end(),
		[&](auto& iter) {
			ASSERT_NO_THROW(test_instance_ptr1->push_back(*iter));
		}
	);

	ASSERT_EQ(test_data.size(), test_instance_ptr1->size());

	ASSERT_NO_THROW(test_instance_ptr2 = std::unique_ptr<Array>(new Array(*test_instance_ptr1)));
	ASSERT_EQ(test_instance_ptr1->size(), test_instance_ptr2->size());
	ASSERT_NO_THROW(test_instance_ptr2->remove(0));
	ASSERT_EQ(test_instance_ptr1->size(), test_instance_ptr2->size() + 1);
}