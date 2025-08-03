#ifndef	AXES_CONTROLLER_HPP
#define	AXES_CONTROLLER_HPP

#include "movement_manager_data.hpp"

namespace manager {
	class AxesController {
	public:
		virtual ~AxesController() noexcept = default;
		virtual void step(const Axis& axis, const Direction& direction, const double duration) = 0;
		virtual void enable() = 0;
		virtual void disable() = 0;
		virtual double get_step_length(const Axis& axis) const = 0;		
	};
}

#endif // AXES_CONTROLLER_HPP