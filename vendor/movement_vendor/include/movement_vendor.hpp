#ifndef	MOVEMENT_VENDOR_HPP
#define	MOVEMENT_VENDOR_HPP

#include <exception>
#include <stdexcept>
#include <string>

#include "axes_controller_config_request.hpp"
#include "linear_movement_request.hpp"
#include "movement_manager.hpp"
#include "movement_vendor_api_request.hpp"
#include "movement_vendor_api_response.hpp"
#include "rotation_movement_request.hpp"
#include "vendor.hpp"
#include "vendor_instance.hpp"

namespace vendor {
	template <typename AxesConfig>
	class MovementVendor: public Vendor<MovementVendorApiRequest, MovementVendorApiResponse> {
	public:
		using MovementManagerInstance = vendor::Instance<manager::MovementManager<AxesConfig>>;

		MovementVendor(
			const MovementManagerInstance& movement_manager
		);
		MovementVendor(const MovementVendor& other) = default;
		MovementVendor& operator=(const MovementVendor&) = delete;

		MovementVendorApiResponse run_api_request(const MovementVendorApiRequest& request) override;
	private:
		MovementManagerInstance m_movement_manager;
		
		MovementVendorApiResponse run_cfg_request(const MovementVendorApiRequest& request);
		MovementVendorApiResponse run_linear_movement_request(const MovementVendorApiRequest& request);
		MovementVendorApiResponse run_circular_movement_request(const MovementVendorApiRequest& request);

		template <typename T>
		static const T& cast_request(const MovementVendorApiRequest& request);
	};

	template <typename AxesConfig>
	inline MovementVendor<AxesConfig>::MovementVendor(
		const MovementManagerInstance& movement_manager
	): m_movement_manager(movement_manager) {}
	
	template <typename AxesConfig>
	inline MovementVendorApiResponse MovementVendor<AxesConfig>::run_api_request(const MovementVendorApiRequest& request) {
		try {
			switch (request.type()) {
			case MovementVendorApiRequest::RequestType::CONFIG:
				return run_cfg_request(request);
			case MovementVendorApiRequest::RequestType::LINEAR_MOVEMENT:
				return run_linear_movement_request(request);
			case MovementVendorApiRequest::RequestType::ROTATIONAL_MOVEMENT:
				return run_circular_movement_request(request);
			default:
				return MovementVendorApiResponse(MovementVendorApiResponse::Result::FAILURE, std::string("unsupported api request received"));
			}
		} catch (const std::exception& e) {
			return MovementVendorApiResponse(MovementVendorApiResponse::Result::FAILURE, std::string(e.what()));
		}
	}

	template <typename AxesConfig>
	inline MovementVendorApiResponse MovementVendor<AxesConfig>::run_cfg_request(const MovementVendorApiRequest& request) {
		auto casted_request = cast_request<AxesControllerConfigApiRequest<AxesConfig>>(request);
		m_movement_manager.get().init(casted_request.axes_cfg());
		return MovementVendorApiResponse(MovementVendorApiResponse::Result::SUCCESS);
	}

	template <typename AxesConfig>
	inline MovementVendorApiResponse MovementVendor<AxesConfig>::run_linear_movement_request(const MovementVendorApiRequest& request) {
		auto casted_request = cast_request<LinearMovementRequest>(request);
		m_movement_manager.get().linear_movement(casted_request.destination(), casted_request.speed());
		return MovementVendorApiResponse(MovementVendorApiResponse::Result::SUCCESS);
	}

	template <typename AxesConfig>
	inline MovementVendorApiResponse MovementVendor<AxesConfig>::run_circular_movement_request(const MovementVendorApiRequest& request) {
		auto casted_request = cast_request<RotationMovementRequest>(request);
		m_movement_manager.get().circular_movement(
			casted_request.destination(),
			casted_request.rotation_center(),
			casted_request.angle(),
			casted_request.speed()
		);
		return MovementVendorApiResponse(MovementVendorApiResponse::Result::SUCCESS);
	}

	template <typename AxesConfig>
	template <typename T>
	inline const T& MovementVendor<AxesConfig>::cast_request(const MovementVendorApiRequest& request) {
		try {
			return dynamic_cast<const T&>(request);
		} catch (...) {
			throw std::runtime_error("failed to cast a movement vendor api request into target type");
		}
	}
}

#endif // MOVEMENT_VENDOR_HPP