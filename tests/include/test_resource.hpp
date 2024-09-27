#ifndef	TEST_RESOURCE_HPP
#define	TEST_RESOURCE_HPP

#include <functional>
#include <stdexcept>
#include <string>

#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"

namespace server_uts {
	class TestResource: public server::Resource {
	public:
		using TestAction = std::function<server::Response(const server::Request&)>;
		TestResource(const std::string& id, const TestAction& action): m_id(id), m_action(action) {
			if (!m_action) {
				throw std::invalid_argument("invalid action received");
			}
		}

		std::string id() const override {
			return m_id;
		}

		server::Response run_request(const server::Request& request) const override {
			return m_action(request);
		}

		server::Resource *clone() const override {
			return new TestResource(*this);
		}
	private:
		std::string m_id;
		TestAction m_action;
	};
}

#endif // TEST_RESOURCE_HPP