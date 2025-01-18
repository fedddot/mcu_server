#ifndef	TEST_PROVIDERS_HPP
#define	TEST_PROVIDERS_HPP

#include <functional>

#include "manager.hpp"

namespace manager_tests {
	template <typename Request, typename Response>
	class TestManager: public manager::Manager<Request, Response> {
	public:
		using Action = std::function<Response(const Request&)>;
		TestManager(const Action& action): m_action(action) {

		}
		TestManager(const TestManager&) = delete;
		TestManager& operator=(const TestManager&) = delete;
		
		Response run(const Request& request) override {
			return m_action(request);
		}
	private:
		Action m_action;
	};
}

#endif // TEST_PROVIDERS_HPP