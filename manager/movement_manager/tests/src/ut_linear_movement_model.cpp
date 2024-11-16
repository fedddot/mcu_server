#include <memory>

#include "gtest/gtest.h"

#include "linear_movement_model.hpp"
#include "vector.hpp"

using namespace manager;

TEST(ut_linear_movement_model, ctor_dtor_sanity) {
	// GIVEN
	const Vector<double> target(10, 20, 30);
	const double speed(3);
	const unsigned int steps_per_length(100);

	// WHEN
	std::unique_ptr<LinearMovementModel> instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance_ptr = std::unique_ptr<LinearMovementModel>(
			new LinearMovementModel(
				target,
				speed,
				steps_per_length
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

static inline void print_vector(const Vector<double>& vector) {
	using Axis = typename Vector<double>::Axis;
	std::cout << "(" << vector.projection(Axis::X) << ", " << vector.projection(Axis::Y) << ", " << vector.projection(Axis::Z) << ")" << std::endl;   
}

TEST(ut_linear_movement_model, evaluate_sanity) {
	// GIVEN
	const Vector<double> target(-1, -45, 3);
	const double speed(3);
	const unsigned int steps_per_length(100);

	// WHEN
	LinearMovementModel instance(target, speed, steps_per_length);
	Vector<double> result(0, 0, 0);
	
	// THEN
	while (!instance.finished()) {
		ASSERT_NO_THROW(result = instance.evaluate());
		print_vector(result);
	}
}