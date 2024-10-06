#ifndef	TEST_RESOURCE_HPP
#define	TEST_RESOURCE_HPP

#include <functional>
#include <stdexcept>

#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"

namespace server_uts {
	class TestResource: public server::Resource {
	public:
		using TestAction = std::function<server::Response(const server::Request&)>;
		TestResource(const TestAction& action): m_action(action) {
			if (!m_action) {
				throw std::invalid_argument("invalid action received");
			}
		}

		server::Response run_request(const server::Request& request) const override {
			return m_action(request);
		}

		server::Resource *clone() const override {
			return new TestResource(*this);
		}
	private:
		TestAction m_action;
	};
}

#endif // TEST_RESOURCE_HPP