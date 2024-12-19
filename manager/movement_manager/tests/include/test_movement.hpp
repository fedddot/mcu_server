#ifndef	TEST_MOVEMENT_HPP
#define	TEST_MOVEMENT_HPP

#include <functional>
#include <stdexcept>

#include "data.hpp"
#include "movement.hpp"

namespace manager_uts {
	class TestMovement: public manager::Movement {
	public:
		using Action = std::function<void(const server::Data&)>;
		TestMovement(const Action& action, const Type& type): m_action(action), m_type(type) {
			if (!m_action) {
				throw std::invalid_argument("invalid action received");
			}
		}
		void perform(const server::Data& cfg) override {
			m_action(cfg);
		}
		Type type() const override {
			return m_type;
		}
	private:
		Action m_action;
		Type m_type;
	};
}

#endif // TEST_MOVEMENT_HPP