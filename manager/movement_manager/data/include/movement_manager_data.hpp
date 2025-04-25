#ifndef	MOVEMENT_MANAGER_DATA_HPP
#define	MOVEMENT_MANAGER_DATA_HPP

#include <map>
#include <stdexcept>

namespace manager {
	enum class Axis: int {
		X = 0,
		Y = 1,
		Z = 2,
	};

	enum class Direction: int {
		NEGATIVE = 0,
		POSITIVE = 1,
	};

	struct AxisStep {
		Axis axis;
		Direction direction;
		double duration;
	};

	class AxesProperties {
	public:
		AxesProperties(
			const double x_step_length,
			const double y_step_length,
			const double z_step_length
		);
		AxesProperties(const AxesProperties&) = default;
		AxesProperties& operator=(const AxesProperties&) = default;
		~AxesProperties() = default;

		double get_step_length(const Axis& axis) const;
	private:
		std::map<Axis, double> m_step_lengths;
	};

	inline AxesProperties::AxesProperties(
		const double x_step_length,
		const double y_step_length,
		const double z_step_length
	): m_step_lengths {
		{Axis::X, x_step_length},
		{Axis::Y, y_step_length},
		{Axis::Z, z_step_length}
	} {
		for (const auto iter : m_step_lengths) {
			if (iter.second <= 0.0) {
				throw std::invalid_argument("step length must be a positive non-zero value");
			}
		}
	}

	inline double AxesProperties::get_step_length(const Axis& axis) const {
		return m_step_lengths.at(axis);
	}
}

#endif // MOVEMENT_MANAGER_DATA_HPP