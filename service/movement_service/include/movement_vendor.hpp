#ifndef	MOVEMENT_SERVICE_HPP
#define	MOVEMENT_SERVICE_HPP

#include <exception>
#include <stdexcept>
#include <string>

#include "axes_controller_config_request.hpp"
#include "linear_movement_request.hpp"
#include "movement_manager.hpp"
#include "movement_service_api_request.hpp"
#include "movement_service_api_response.hpp"
#include "rotation_movement_request.hpp"
#include "service.hpp"
#include "service_instance.hpp"

namespace service {
	template <typename AxesConfig>
	class MovementService: public Service<MovementServiceApiRequest, MovementServiceApiResponse> {
	public:
		using MovementManagerInstance = service::Instance<manager::MovementManager<AxesConfig>>;

		MovementService(
			const MovementManagerInstance& movement_manager
		);
		MovementService(const MovementService& other) = default;
		MovementService& operator=(const MovementService&) = delete;

		MovementServiceApiResponse run_api_request(const MovementServiceApiRequest& request) override;
	private:
		MovementManagerInstance m_movement_manager;
		
		MovementServiceApiResponse run_cfg_request(const MovementServiceApiRequest& request);
		MovementServiceApiResponse run_linear_movement_request(const MovementServiceApiRequest& request);
		MovementServiceApiResponse run_circular_movement_request(const MovementServiceApiRequest& request);

		template <typename T>
		static const T& cast_request(const MovementServiceApiRequest& request);
	};

	template <typename AxesConfig>
	inline MovementService<AxesConfig>::MovementService(
		const MovementManagerInstance& movement_manager
	): m_movement_manager(movement_manager) {}
	
	template <typename AxesConfig>
	inline MovementServiceApiResponse MovementService<AxesConfig>::run_api_request(const MovementServiceApiRequest& request) {
		try {
			switch (request.type()) {
			case MovementServiceApiRequest::RequestType::CONFIG:
				return run_cfg_request(request);
			case MovementServiceApiRequest::RequestType::LINEAR_MOVEMENT:
				return run_linear_movement_request(request);
			case MovementServiceApiRequest::RequestType::ROTATIONAL_MOVEMENT:
				return run_circular_movement_request(request);
			default:
				return MovementServiceApiResponse(MovementServiceApiResponse::Result::FAILURE, std::string("unsupported api request received"));
			}
		} catch (const std::exception& e) {
			return MovementServiceApiResponse(MovementServiceApiResponse::Result::FAILURE, std::string(e.what()));
		}
	}

	template <typename AxesConfig>
	inline MovementServiceApiResponse MovementService<AxesConfig>::run_cfg_request(const MovementServiceApiRequest& request) {
		auto casted_request = cast_request<AxesControllerConfigApiRequest<AxesConfig>>(request);
		m_movement_manager.get().init(casted_request.axes_cfg());
		return MovementServiceApiResponse(MovementServiceApiResponse::Result::SUCCESS);
	}

	template <typename AxesConfig>
	inline MovementServiceApiResponse MovementService<AxesConfig>::run_linear_movement_request(const MovementServiceApiRequest& request) {
		auto casted_request = cast_request<LinearMovementRequest>(request);
		m_movement_manager.get().linear_movement(casted_request.destination(), casted_request.speed());
		return MovementServiceApiResponse(MovementServiceApiResponse::Result::SUCCESS);
	}

	template <typename AxesConfig>
	inline MovementServiceApiResponse MovementService<AxesConfig>::run_circular_movement_request(const MovementServiceApiRequest& request) {
		auto casted_request = cast_request<RotationMovementRequest>(request);
		m_movement_manager.get().circular_movement(
			casted_request.destination(),
			casted_request.rotation_center(),
			casted_request.angle(),
			casted_request.speed()
		);
		return MovementServiceApiResponse(MovementServiceApiResponse::Result::SUCCESS);
	}

	template <typename AxesConfig>
	template <typename T>
	inline const T& MovementService<AxesConfig>::cast_request(const MovementServiceApiRequest& request) {
		try {
			return dynamic_cast<const T&>(request);
		} catch (...) {
			throw std::runtime_error("failed to cast a movement service api request into target type");
		}
	}
}

#endif // MOVEMENT_SERVICE_HPP