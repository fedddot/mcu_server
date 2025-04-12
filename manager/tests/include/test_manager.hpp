#ifndef	TEST_PROVIDERS_HPP
#define	TEST_PROVIDERS_HPP

#include <functional>

#include "manager.hpp"
#include "clonable_manager.hpp"

namespace manager_tests {
	template <typename Request, typename Response>
	class TestManager: public manager::ClonableManager<Request, Response> {
	public:
		using Action = std::function<Response(const Request&)>;
		TestManager(const Action& action): m_action(action) {

		}
		TestManager(const TestManager&) = default;
		TestManager& operator=(const TestManager&) = default;
		
		Response run(const Request& request) override {
			return m_action(request);
		}
		manager::Manager<Request, Response> *clone() const {
			return new TestManager(*this);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_PROVIDERS_HPP