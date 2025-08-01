#include "gtest/gtest.h"

#include "ring_queue.hpp"

using namespace ipc;

TEST(ut_ring_queue, sanity) {
	// GIVEN
	const auto buff_size = 3UL;
	using TestBuffer = RingDataBuffer<char, buff_size>;
	const char test_data[] {'a', 'b', 'c'};

	// WHEN
	TestBuffer instance;

	// THEN
	ASSERT_EQ(instance.size(), 0UL);
	ASSERT_EQ(instance.capacity(), buff_size);
	for (const auto& elem : test_data) {
		instance.push_back(elem);
		const auto elem_index = &elem - test_data;
		ASSERT_EQ(instance.size(), elem_index + 1);
		ASSERT_EQ(instance.get(elem_index), elem);
	}
	ASSERT_EQ(instance.size(), buff_size);
	ASSERT_THROW(instance.push_back('d'), std::overflow_error);
	for (const auto& elem : test_data) {
		const auto popped_elem = instance.pop_first();
		ASSERT_EQ(popped_elem, elem);
		ASSERT_EQ(instance.size(), buff_size - (&elem - test_data) - 1);
	}
	ASSERT_EQ(instance.size(), 0UL);
	ASSERT_THROW(instance.pop_first(), std::runtime_error);
}