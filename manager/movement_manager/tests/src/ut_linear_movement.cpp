#include <iostream>
#include <vector>

#include "gtest/gtest.h"

#include "linear_movement.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"

using namespace manager;

static Vector<double> integrate_path(const std::vector<AxisStep>& steps, const AxesProperties& axes_properties);
static void print_vector(const Vector<double>& vector);
static void print_axis_properties(const AxesProperties& axes_properties);
static void print_allowed_absolute_error(const double allowed_absolute_error);
static std::string axis_to_string(const Axis& axis);

TEST(ut_linear_movement, ctor_dtor_sanity) {
	// GIVEN
	const auto destination = Vector<double>(10.0, 20.0, 30.0);
	const auto axes_properties = AxesProperties(3.0, 4.0, 5.0);
	
	// WHEN
	LinearMovement *instance(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance = new LinearMovement(
			destination,
			axes_properties
		)
	);
	ASSERT_NE(instance, nullptr);
	ASSERT_NO_THROW(delete instance);
}


TEST(ut_linear_movement, evaluate_sanity) {
	// GIVEN
	struct TestCase {
		const Vector<double> destination;
		const AxesProperties axes_properties;
		const double allowed_absolute_error;
	};
	const auto coarse_error = 2.0;
	const auto fine_error = 0.008;
	const auto destinations = {
		TestCase {
			Vector<double>(10.0, 20.0, 30.0),
			AxesProperties(3.0, 4.0, 5.0),
			coarse_error
		},
		TestCase {
			Vector<double>(-10.0, 20.0, -30.0),
			AxesProperties(3.0, 4.0, 5.0),
			coarse_error
		},
		TestCase {
			Vector<double>(35.761, 300.0, 90.86423),
			AxesProperties(0.005, 0.005, 0.005),
			fine_error
		},
		TestCase {
			Vector<double>(35.761, -300.0, -90.86423),
			AxesProperties(0.005, 0.005, 0.005),
			fine_error
		},
	};

	for (const auto& test_case: destinations) {
		std::cout << std::endl << std::endl << "Running test case" << std::endl;
		print_vector(test_case.destination);
		print_axis_properties(test_case.axes_properties);
		print_allowed_absolute_error(test_case.allowed_absolute_error);

		// WHEN
		LinearMovement instance(
			test_case.destination,
			test_case.axes_properties
		);
		auto result = std::vector<AxisStep>();

		// THEN
		ASSERT_NO_THROW(
			result = instance.evaluate_steps();
		);
		const auto path = integrate_path(result, test_case.axes_properties);
		std::cout << "Path: " << std::endl;
		print_vector(path);
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			// WHEN
			const auto path_projection = path.get(axis);
			const auto destination_projection = test_case.destination.get(axis);
			const auto allowed_error = test_case.allowed_absolute_error;
			const auto error = std::abs(path_projection - destination_projection);
			std::cout << "axis " << axis_to_string(axis) << ": allowed error: " << allowed_error << ", error: " << error << std::endl;
			// THEN
			ASSERT_LE(error, allowed_error);
		}
	}
}

inline Vector<double> integrate_path(const std::vector<AxisStep>& steps, const AxesProperties& axes_properties) {
	auto add_path = [](const Vector<double>& current, const AxisStep& step, const AxesProperties& axes_properties) {
		auto increment = axes_properties.get_step_length(step.axis);
		if (step.direction == Direction::NEGATIVE) {
			increment = -increment;
		}
		auto result = current;
		result.set(step.axis, result.get(step.axis) + increment);
		return result;
	};
	auto path = Vector<double>(0.0, 0.0, 0.0);
	for (const auto& step : steps) {
		path = add_path(path, step, axes_properties);
	}
	return path;
}

inline std::string axis_to_string(const Axis& axis) {
	switch (axis) {
		case Axis::X: return "X";
		case Axis::Y: return "Y";
		case Axis::Z: return "Z";
		default: throw std::invalid_argument("unsupported axis");
	}
}

inline void print_vector(const Vector<double>& vector) {
	std::cout << "Vector: {" << std::endl;
	for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
		std::cout << '\t' << axis_to_string(axis) << ": " << vector.get(axis) << "," << std::endl;
	}
	std::cout << "}" << std::endl;
}

inline void print_axis_properties(const AxesProperties& axes_properties) {
	std::cout << "Axes properties: {" << std::endl;
	for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
		std::cout << '\t' << axis_to_string(axis) << " (step length): " << axes_properties.get_step_length(axis) << "," << std::endl;
	}
	std::cout << "}" << std::endl;
}

inline void print_allowed_absolute_error(const double allowed_absolute_error) {
	std::cout << "Allowed absolute error: " << allowed_absolute_error << std::endl;
}