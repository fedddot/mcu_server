#include <memory>

#include "gtest/gtest.h"

#include "circular_movement_model.hpp"
#include "vector.hpp"

using namespace manager;

TEST(ut_circular_movement_model, ctor_dtor_sanity) {
	// GIVEN
	const Vector<int> target(10, 20, 30);
	const Vector<int> rotation_center(5, 10, 20);
	const CircularMovementModel::Direction direction(CircularMovementModel::Direction::CCW);
	const unsigned int speed(3); // 3 length units / 1 time unit

	// WHEN
	std::unique_ptr<CircularMovementModel> instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance_ptr = std::unique_ptr<CircularMovementModel>(
			new CircularMovementModel(
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

static inline void print_vector(const Vector<int>& vector) {
	using Axis = typename Vector<int>::Axis;
	std::cout << "(" << vector.projection(Axis::X) << ", " << vector.projection(Axis::Y) << ", " << vector.projection(Axis::Z) << ")" << std::endl;   
}

TEST(ut_circular_movement_model, evaluate_sanity) {
	// GIVEN
	const Vector<int> target(10, 10, 0);
	const Vector<int> rotation_center(10, 0, 0);
	const auto direction(CircularMovementModel::Direction::CCW);
	const unsigned int speed(3); // 3 length units / 1 time unit

	// WHEN
	CircularMovementModel instance(target, rotation_center, direction, speed);
	Vector<int> result(0, 0, 0);
	const auto tmax(instance.tmax());
	
	// THEN
	for (auto t = 0; t <= instance.tmax(); ++t) {
		ASSERT_NO_THROW(result = instance.evaluate(t));
		print_vector(result);
	}
}