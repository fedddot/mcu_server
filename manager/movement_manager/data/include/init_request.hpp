#ifndef	INIT_REQUEST_HPP
#define	INIT_REQUEST_HPP

#include "movement_manager_request.hpp"

namespace manager {
	template <typename AxesConfig>
	class InitRequest: public MovementManagerRequest {
	public:
		InitRequest(const AxesConfig& axis_controller_config);
		InitRequest(const InitRequest&) = default;
		InitRequest& operator=(const InitRequest&) = default;
		RequestType type() const override;
		AxesConfig axis_controller_config() const;
	private:
		AxesConfig m_axis_controller_config;
	};

	template <typename AxesConfig>
	inline InitRequest<AxesConfig>::InitRequest(const AxesConfig& axis_controller_config): m_axis_controller_config(axis_controller_config) {
	}

	template <typename AxesConfig>
	inline MovementManagerRequest::RequestType InitRequest<AxesConfig>::type() const {
		return RequestType::INIT;
	}

	template <typename AxesConfig>
	inline AxesConfig InitRequest<AxesConfig>::axis_controller_config() const {
		return m_axis_controller_config;
	}
}

#endif // INIT_REQUEST_HPP