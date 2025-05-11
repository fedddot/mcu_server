#ifndef	TEST_AXES_CONTROLLER_HPP
#define	TEST_AXES_CONTROLLER_HPP

#include <functional>
#include <stdexcept>

#include "axes_controller.hpp"
#include "movement_manager_data.hpp"

namespace manager {
	class TestAxesController: public AxesController {
	public:
		using Action = std::function<void(const AxisStep& step)>;
		TestAxesController(const Action& action): m_action(action) {
			if (!m_action) {
				throw std::invalid_argument("invalid action received");
			}
		}
		
		void step(const AxisStep& step) override {
			m_action(step);
		}
		
		void enable() override {

		}

		void disable() override {

		}

		AxesController *clone() const override {
			return new TestAxesController(*this);
		};
	private:
		Action m_action;
	};
}

#endif // TEST_AXES_CONTROLLER_HPP