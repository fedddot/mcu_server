#ifndef	TEST_PROVIDERS_HPP
#define	TEST_PROVIDERS_HPP

#include <functional>

#include "manager.hpp"

namespace manager_tests {
	template <typename Trequest, typename Tresponse>
	class TestManager: public manager::Manager<Trequest, Tresponse> {
	public:
		using Action = std::function<Tresponse(const Trequest&)>;
		TestManager(const Action& action): m_action(action) {

		}
		TestManager(const TestManager&) = delete;
		TestManager& operator=(const TestManager&) = delete;
		
		Tresponse run(const Trequest& request) override {
			return m_action(request);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_PROVIDERS_HPP