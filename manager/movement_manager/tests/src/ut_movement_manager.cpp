#include "gtest/gtest.h"

#include "movement_manager.hpp"

using namespace manager;

TEST(ut_movement_manager, ctor_dtor_sanity) {
	// WHEN
	MovementManager *instance(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance = new MovementManager()
	);
	ASSERT_NE(instance, nullptr);
	ASSERT_NO_THROW(delete instance);
}
