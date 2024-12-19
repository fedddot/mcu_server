#include <memory>

#include "gtest/gtest.h"

#include "circular_movement_model.hpp"
#include "vector.hpp"

using namespace manager;

// TEST(ut_circular_movement_model, ctor_dtor_sanity) {
// 	// GIVEN
// 	const Vector<double> target(10, 10, 0);
// 	const Vector<double> rotation_center(10, 0, 0);
// 	const CircularMovementModel::Direction direction(CircularMovementModel::Direction::CW);
// 	const double speed(3);
// 	const unsigned int steps_per_length(100);

// 	// WHEN
// 	std::unique_ptr<CircularMovementModel> instance_ptr(nullptr);

// 	// THEN
// 	ASSERT_NO_THROW(
// 		instance_ptr = std::unique_ptr<CircularMovementModel>(
// 			new CircularMovementModel(
// 				target,
// 				rotation_center,
// 				direction,
// 				speed,
// 				steps_per_length
// 			)
// 		)
// 	);
// 	ASSERT_NE(nullptr, instance_ptr);
// 	ASSERT_NO_THROW(instance_ptr = nullptr);
// }

static inline void print_vector(const Vector<double>& vector) {
	using Axis = typename Vector<double>::Axis;
	std::cout << "(" << vector.projection(Axis::X) << ", " << vector.projection(Axis::Y) << ", " << vector.projection(Axis::Z) << ")" << std::endl;   
}

TEST(ut_circular_movement_model, evaluate_sanity) {
	// GIVEN
	const Vector<double> target(2, 0, 0);
	const Vector<double> rotation_center(1, 0, 0);
	const CircularMovementModel::Direction direction(CircularMovementModel::Direction::CCW);
	const double speed(3);
	const unsigned int steps_per_length(100);

	// WHEN
	CircularMovementModel instance(target, rotation_center, direction, speed, steps_per_length);
	Vector<double> result(0, 0, 0);
	const auto dt(instance.dt());
	
	// THEN
	while (true) {
		ASSERT_NO_THROW(result = instance.evaluate());
		print_vector(result);
		if (instance.finished()) {
			break;
		}
	}
}