#ifndef	TEST_SERVICE_HPP
#define	TEST_SERVICE_HPP

#include <functional>
#include <stdexcept>

#include "service.hpp"

namespace service {
	template <typename ApiRequest, typename ApiResponse>
	class TestService: public Service<ApiRequest, ApiResponse> {
	public:
		using TestAction = std::function<ApiResponse(const ApiRequest&)>;

		TestService(const TestAction& test_action);
		TestService(const TestService&) = default;
		TestService& operator=(const TestService&) = default;

		ApiResponse run_api_request(const ApiRequest& request) override;
	private:
		TestAction m_test_action;
	};

	template <typename ApiRequest, typename ApiResponse>
	inline TestService<ApiRequest, ApiResponse>::TestService(const TestAction& test_action): m_test_action(test_action) {
		if (!m_test_action) {
			throw std::invalid_argument("invalid test action received");
		}
	}

	template <typename ApiRequest, typename ApiResponse>
	inline ApiResponse TestService<ApiRequest, ApiResponse>::run_api_request(const ApiRequest& request) {
		return m_test_action(request);
	}
}

#endif // TEST_SERVICE_HPP