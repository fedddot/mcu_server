#ifndef	CUSTOM_MANAGER_HPP
#define	CUSTOM_MANAGER_HPP

#include <functional>
#include <stdexcept>

#include "manager.hpp"

namespace manager {
	template <typename Request, typename Response>
	class CustomManager: public Manager<Request, Response> {
	public:
		using CustomAction = std::function<Response(const Request&)>;
		CustomManager(const CustomAction& custom_action);
		CustomManager(const CustomManager&) = default;
		CustomManager& operator=(const CustomManager&) = default;

		Response run(const Request& request) override;
	private:
		CustomAction m_custom_action;
	};

	template <typename Request, typename Response>
	inline CustomManager<Request, Response>::CustomManager(const CustomAction& custom_action): m_custom_action(custom_action) {
		if (!m_custom_action) {
			throw std::invalid_argument("invalid custom action received");
		}
	}

	template <typename Request, typename Response>
	inline Response CustomManager<Request, Response>::run(const Request& request) {
		return m_custom_action(request);
	}
}

#endif // CUSTOM_MANAGER_HPP