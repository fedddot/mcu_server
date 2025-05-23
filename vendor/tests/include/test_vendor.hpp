#ifndef	TEST_VENDOR_HPP
#define	TEST_VENDOR_HPP

#include <functional>
#include <stdexcept>

#include "vendor.hpp"

namespace vendor {
	template <typename ApiRequest, typename ApiResponse>
	class TestVendor: public Vendor<ApiRequest, ApiResponse> {
	public:
		using TestAction = std::function<ApiResponse(const ApiRequest&)>;

		TestVendor(const TestAction& test_action);
		TestVendor(const TestVendor&) = default;
		TestVendor& operator=(const TestVendor&) = default;

		ApiResponse run_api_request(const ApiRequest& request) override;
	private:
		TestAction m_test_action;
	};

	template <typename ApiRequest, typename ApiResponse>
	inline TestVendor<ApiRequest, ApiResponse>::TestVendor(const TestAction& test_action): m_test_action(test_action) {
		if (!m_test_action) {
			throw std::invalid_argument("invalid test action received");
		}
	}

	template <typename ApiRequest, typename ApiResponse>
	inline ApiResponse TestVendor<ApiRequest, ApiResponse>::run_api_request(const ApiRequest& request) {
		return m_test_action(request);
	}
}

#endif // TEST_VENDOR_HPP