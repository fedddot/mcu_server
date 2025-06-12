#ifndef CIRCULAR_MOVEMENT_HPP
#define CIRCULAR_MOVEMENT_HPP

#include <cmath>
#include <map>
#include <optional>
#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"

namespace manager {
class CircularMovement {
public:
    using AxesStepLengths = std::map<Axis, double>;
    CircularMovement(
        const Vector<double>& destination,
        const Vector<double>& rotation_center,
        const AxesStepLengths& axes_step_lengths,
        double angle,
        double speed
    );
    CircularMovement(const CircularMovement&) = delete;
    CircularMovement& operator=(const CircularMovement&) = delete;

    std::optional<AxisStep> next_step();
private:
    Vector<double> m_destination;
    Vector<double> m_rotation_center;
    AxesStepLengths m_axes_step_lengths;
    double m_angle;
    double m_speed;
    double m_step_duration;
    double m_total_steps;
    double m_current_step;
    Vector<double> m_current;

    static double evaluate_step_duration(double arc_length, double speed, std::size_t steps);
    static std::size_t evaluate_steps_number(double arc_length, double step_length);
    Vector<double> calculate_arc_point(double t) const;
    AxisStep evaluate_axis_step(const Vector<double>& from, const Vector<double>& to) const;
};

inline CircularMovement::CircularMovement(
    const Vector<double>& destination,
    const Vector<double>& rotation_center,
    const AxesStepLengths& axes_step_lengths,
    double angle,
    double speed
): m_destination(destination), m_rotation_center(rotation_center), m_axes_step_lengths(axes_step_lengths), m_angle(angle), m_speed(speed), m_current(0.0, 0.0, 0.0), m_current_step(0)
{
    // Calculate radius and arc length
    double radius = std::sqrt(
        std::pow(rotation_center.get(Axis::X), 2) +
        std::pow(rotation_center.get(Axis::Y), 2) +
        std::pow(rotation_center.get(Axis::Z), 2)
    );
    double arc_length = radius * (angle * M_PI / 180.0);
    double step_length = axes_step_lengths.at(Axis::X); // Assume uniform step for simplicity
    m_total_steps = evaluate_steps_number(arc_length, step_length);
    m_step_duration = evaluate_step_duration(arc_length, speed, m_total_steps);
}

inline std::optional<AxisStep> CircularMovement::next_step() {
    if (m_current_step >= m_total_steps) {
        return std::nullopt;
    }
    double t1 = m_current_step / m_total_steps;
    double t2 = (m_current_step + 1) / m_total_steps;
    Vector<double> from = calculate_arc_point(t1);
    Vector<double> to = calculate_arc_point(t2);
    AxisStep step = evaluate_axis_step(from, to);
    m_current = to;
    m_current_step++;
    return step;
}

inline double CircularMovement::evaluate_step_duration(double arc_length, double speed, std::size_t steps) {
    if (speed <= 0.0 || steps == 0) return 0.0;
    return (arc_length / speed) / steps;
}

inline std::size_t CircularMovement::evaluate_steps_number(double arc_length, double step_length) {
    if (step_length <= 0.0) return 0;
    return static_cast<std::size_t>(std::ceil(arc_length / step_length));
}

inline Vector<double> CircularMovement::calculate_arc_point(double t) const {
    // Parametric equation for a circle in the plane defined by rotation_center and destination
    // For simplicity, assume rotation around Z axis
    double radius = std::sqrt(
        std::pow(m_rotation_center.get(Axis::X), 2) +
        std::pow(m_rotation_center.get(Axis::Y), 2)
    );
    double theta = t * (m_angle * M_PI / 180.0);
    double x = radius * std::cos(theta);
    double y = radius * std::sin(theta);
    double z = 0.0;
    return Vector<double>(x, y, z);
}

inline AxisStep CircularMovement::evaluate_axis_step(const Vector<double>& from, const Vector<double>& to) const {
    // Find the axis with the largest movement
    double max_delta = 0.0;
    Axis best_axis = Axis::X;
    for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
        double delta = to.get(axis) - from.get(axis);
        if (std::abs(delta) > std::abs(max_delta)) {
            max_delta = delta;
            best_axis = axis;
        }
    }
    Direction dir = (max_delta >= 0) ? Direction::POSITIVE : Direction::NEGATIVE;
    double duration = m_step_duration;
    return AxisStep{best_axis, dir, duration};
}

} // namespace manager

#endif // CIRCULAR_MOVEMENT_HPP
