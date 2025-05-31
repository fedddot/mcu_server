#ifndef	MOVEMENT_MANAGER_HPP
#define	MOVEMENT_MANAGER_HPP

#include <functional>
#include <optional>
#include <stdexcept>

#include "axes_controller.hpp"
#include "manager_instance.hpp"
#include "linear_movement.hpp"
#include "manager.hpp"
#include "movement_manager_data.hpp"

namespace manager {
	template <typename AxesConfig>
	class MovementManager: public Manager {
	public:
		using AxesControllerCreator = std::function<Instance<AxesController>(const AxesConfig&)>;
		MovementManager(
			const AxesControllerCreator& axes_controller_ctor
		);
		MovementManager(const MovementManager& other) = default;
		MovementManager& operator=(const MovementManager&) = delete;

		void init(const AxesConfig& axes_config);
		void linear_movement(const Vector<double>& destination, const double speed);
		void circular_movement(const Vector<double>& destination, const Vector<double>& rotation_center, const double angle, const double speed);
	private:
		AxesControllerCreator m_axes_controller_ctor;
		std::optional<Instance<AxesController>> m_axes_controller;
	};

	template <typename AxesConfig>
	inline MovementManager<AxesConfig>::MovementManager(
		const AxesControllerCreator& axes_controller_ctor
	): m_axes_controller_ctor(axes_controller_ctor), m_axes_controller(std::nullopt) {
		if (!m_axes_controller_ctor) {
			throw std::invalid_argument("invalid axes_controller_ctor received");
		}
	}

	template <typename AxesConfig>
	inline void MovementManager<AxesConfig>::init(const AxesConfig& axes_config) {
		m_axes_controller = std::nullopt;
		m_axes_controller = m_axes_controller_ctor(axes_config);
	}

	template <typename AxesConfig>
	inline void MovementManager<AxesConfig>::linear_movement(const Vector<double>& destination, const double speed) {
		if (!m_axes_controller) {
			throw std::runtime_error("axes controller is not initialized");
		}
		auto axes_step_lengths = LinearMovement::AxesStepLengths();
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			axes_step_lengths[axis] = m_axes_controller->get().get_step_length(axis);
		}
		auto movement = LinearMovement(
			destination,
			axes_step_lengths,
			speed
		);
		m_axes_controller->get().enable();
		while (true) {
			const auto step = movement.next_step();
			if (!step) {
				break;
			}
			m_axes_controller->get().step(step->axis(), step->direction(), step->duration());
		}
		m_axes_controller->get().disable();
	}
	
	template <typename AxesConfig>
	inline void MovementManager<AxesConfig>::circular_movement(const Vector<double>& destination, const Vector<double>& rotation_center, const double angle, const double speed) {
		throw std::runtime_error("MovementManager<AxesConfig>::circular_movement is not implemented");
	}
}

#endif // MOVEMENT_MANAGER_HPP