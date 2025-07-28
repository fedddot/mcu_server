#include "gtest/gtest.h"

#include "ring_data_buffer.hpp"

using namespace tools;

TEST(ut_ring_data_buffer, ctor_dtor_sanity) {
	// WHEN
	RingDataBuffer *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new RingDataBuffer()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}