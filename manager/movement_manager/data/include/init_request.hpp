#ifndef	INIT_REQUEST_HPP
#define	INIT_REQUEST_HPP

#include "movement_manager_request.hpp"

namespace manager {
	template <typename AxisControllerConfig>
	class InitRequest: public MovementManagerRequest {
	public:
		InitRequest(const AxisControllerConfig& axis_controller_config);
		InitRequest(const InitRequest&) = default;
		InitRequest& operator=(const InitRequest&) = default;
		RequestType type() const override;
		AxisControllerConfig axis_controller_config() const;
	private:
		AxisControllerConfig m_axis_controller_config;
	};

	template <typename AxisControllerConfig>
	inline InitRequest<AxisControllerConfig>::InitRequest(const AxisControllerConfig& axis_controller_config): m_axis_controller_config(axis_controller_config) {
	}

	template <typename AxisControllerConfig>
	inline MovementManagerRequest::RequestType InitRequest<AxisControllerConfig>::type() const {
		return RequestType::INIT;
	}

	template <typename AxisControllerConfig>
	inline AxisControllerConfig InitRequest<AxisControllerConfig>::axis_controller_config() const {
		return m_axis_controller_config;
	}
}

#endif // INIT_REQUEST_HPP