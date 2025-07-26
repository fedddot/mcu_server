#ifndef	AXES_CONTROLLER_CONFIG_REQUEST_HPP
#define	AXES_CONTROLLER_CONFIG_REQUEST_HPP

#include "movement_service_api_request.hpp"

namespace service {
	template <typename AxesConfig>
	class AxesControllerConfigApiRequest: public MovementServiceApiRequest {
	public:
		AxesControllerConfigApiRequest(const AxesConfig& axes_cfg): m_axes_cfg(axes_cfg) {

		}
		AxesControllerConfigApiRequest(const AxesControllerConfigApiRequest&) = default;
		AxesControllerConfigApiRequest& operator=(const AxesControllerConfigApiRequest&) = default;
		
		RequestType type() const override {
			return RequestType::CONFIG;
		}
		AxesConfig axes_cfg() const {
			return m_axes_cfg;
		}
	private:
		AxesConfig m_axes_cfg;
	};
}

#endif // AXES_CONTROLLER_CONFIG_REQUEST_HPP