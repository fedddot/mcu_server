#include <memory>

#include "gtest/gtest.h"

#include "circular_movement_model.hpp"
#include "vector.hpp"

using namespace manager;

using LengthUnit = int;
using TimeUnit = unsigned int;
using SpeedUnit = unsigned int;
using TestMovement = CircularMovementModel<LengthUnit, TimeUnit, SpeedUnit>;

TEST(ut_circular_movement_model, ctor_dtor_sanity) {
	// GIVEN
	const Vector<LengthUnit> target(10, 20, 30);
	const Vector<LengthUnit> rotation_center(5, 10, 20);
	const TestMovement::Direction direction(TestMovement::Direction::CCW);
	const SpeedUnit speed(3); // 3 length units / 1 time unit

	// WHEN
	std::unique_ptr<TestMovement> instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance_ptr = std::unique_ptr<TestMovement>(
			new TestMovement(
				target,
				rotation_center,
				direction,
				speed
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_circular_movement_model, evaluate_sanity) {
	// GIVEN
	const Vector<LengthUnit> target(10, 20, 30);
	const Vector<LengthUnit> rotation_center(5, 10, 20);
	const TestMovement::Direction direction(TestMovement::Direction::CCW);
	const SpeedUnit speed(3); // 3 length units / 1 time unit
	const TimeUnit time(2);

	// WHEN
	TestMovement instance(target, rotation_center, direction, speed);
	Vector<LengthUnit> result(0, 0, 0);
	
	// THEN
	ASSERT_NO_THROW(result = instance.evaluate(time));
}