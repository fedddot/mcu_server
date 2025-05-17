#ifndef	TEST_VENDOR_HPP
#define	TEST_VENDOR_HPP

#include <functional>

#include "vendor_api_request.hpp"
#include "vendor_api_response.hpp"
#include "vendor_instance.hpp"
#include "vendor.hpp"

namespace vendor {
	template <typename ManagerId, typename Payload>
	class TestVendor: public Vendor<ManagerId, Payload> {
	public:
		using TestAction = std::function<Instance<ApiResponse>(const ApiRequest<ManagerId, Payload>&)>;

		TestVendor(const TestAction& test_action);
		TestVendor(const TestVendor&) = default;
		TestVendor& operator=(const TestVendor&) = default;

		Instance<ApiResponse> run_api_request(const ApiRequest<ManagerId, Payload>& request) override;
	private:
		TestAction m_test_action;
	};

	template <typename ManagerId, typename Payload>
	inline TestVendor<ManagerId, Payload>::TestVendor(const TestAction& test_action): m_test_action(test_action) {
		if (!m_test_action) {
			throw std::invalid_argument("invalid test action received");
		}
	}

	template <typename ManagerId, typename Payload>
	inline Instance<ApiResponse> TestVendor<ManagerId, Payload>::run_api_request(const ApiRequest<ManagerId, Payload>& request) {
		return m_test_action(request);
	}
}

#endif // TEST_VENDOR_HPP