#include "gtest/gtest.h"

#include "linear_movement.hpp"

using namespace manager;

TEST(ut_linear_movement, ctor_dtor_sanity) {
	// GIVEN
	const auto destination = Vector<double>(10.0, 20.0, 30.0);
	const auto basis = Vector<double>(3.0, 4.0, 5.0);
	const auto speed = double(1.3);

	// WHEN
	LinearMovement *instance(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance = new LinearMovement(
			destination,
			basis,
			speed
		)
	);
	ASSERT_NE(instance, nullptr);
	ASSERT_NO_THROW(delete instance);
}

TEST(ut_linear_movement, evaluate_sanity) {
	// GIVEN
	const auto destination = Vector<double>(10.0, 20.0, 30.0);
	const auto basis = Vector<double>(3.0, 4.0, 5.0);
	const auto speed = double(1.3);

	// WHEN
	LinearMovement instance(
		destination,
		basis,
		speed
	);
	auto result = std::vector<Axis>();

	// THEN
	ASSERT_NO_THROW(
		result = instance.evaluate_steps();
	);
}