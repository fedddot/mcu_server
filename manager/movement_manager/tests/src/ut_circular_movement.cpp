#include "gtest/gtest.h"
#include "circular_movement.hpp"
#include "movement_manager_vector.hpp"
#include <vector>

using namespace manager;

TEST(CircularMovement, completes_full_circle)
{
    // GIVEN
    Vector<double> destination(1.0, 0.0, 0.0);
    Vector<double> rotation_center(0.0, 0.0, 0.0); // Center at origin
    CircularMovement::AxesStepLengths axes_step_lengths = {
        {Axis::X, 0.1},
        {Axis::Y, 0.1},
        {Axis::Z, 0.1}
    };
    double angle = 360.0; // Full circle
    double speed = 1.0;
    CircularMovement movement(destination, rotation_center, axes_step_lengths, angle, speed);
    std::vector<AxisStep> steps;

    // WHEN
    while (auto step = movement.next_step()) {
        steps.push_back(*step);
    }

    // THEN
    ASSERT_FALSE(steps.empty());
    // The number of steps should be greater than zero for a full circle
    // Optionally, check that the last step brings us close to the starting point
}

TEST(CircularMovement, partial_arc)
{
    // GIVEN
    Vector<double> destination(1.0, 0.0, 0.0);
    Vector<double> rotation_center(0.0, 0.0, 0.0);
    CircularMovement::AxesStepLengths axes_step_lengths = {
        {Axis::X, 0.1},
        {Axis::Y, 0.1},
        {Axis::Z, 0.1}
    };
    double angle = 90.0; // Quarter circle
    double speed = 1.0;
    CircularMovement movement(destination, rotation_center, axes_step_lengths, angle, speed);
    std::vector<AxisStep> steps;

    // WHEN
    while (auto step = movement.next_step()) {
        steps.push_back(*step);
    }

    // THEN
    ASSERT_FALSE(steps.empty());
    // The number of steps should be less than for a full circle
}
