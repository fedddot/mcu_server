#ifndef	TEST_AXES_CONTROLLER_HPP
#define	TEST_AXES_CONTROLLER_HPP

#include <functional>
#include <stdexcept>

#include "axes_controller.hpp"
#include "movement_manager_data.hpp"

namespace manager {
	class TestAxesController: public AxesController {
	public:
		using Action = std::function<void(const Axis& axis, const Direction& direction, const double duration)>;
		TestAxesController(const Action& action): m_action(action) {
			if (!m_action) {
				throw std::invalid_argument("invalid action received");
			}
		}
		
		void step(const Axis& axis, const Direction& direction, const double duration) override {
			m_action(axis, direction, duration);
		}
		
		void enable() override {

		}

		void disable() override {

		}
		
		double get_step_length(const Axis& axis) const override {
			return 0.1;
		}
	private:
		Action m_action;
	};
}

#endif // TEST_AXES_CONTROLLER_HPP