#ifndef	MOVEMENT_MANAGER_HPP
#define	MOVEMENT_MANAGER_HPP

#include <cmath>
#include <functional>
#include <optional>
#include <stdexcept>

#include "axes_controller.hpp"
#include "manager_instance.hpp"
#include "linear_movement.hpp"
#include "manager.hpp"
#include "movement_manager_data.hpp"

namespace manager {
	template <typename AxisControllerConfig>
	class MovementManager: public Manager {
	public:
		using AxesControllerCreator = std::function<Instance<AxesController>(const AxisControllerConfig&)>;
		MovementManager(
			const AxesControllerCreator& axes_controller_ctor,
			const AxesProperties& axes_properties
		);
		MovementManager(const MovementManager& other) = default;
		MovementManager& operator=(const MovementManager&) = delete;

		void init(const AxisControllerConfig& axes_config);
		void linear_movement(const Vector<double>& destination, const double speed);
		void circular_movement(const Vector<double>& destination, const Vector<double>& rotation_center, const double speed);
	private:
		AxesControllerCreator m_axes_controller_ctor;
		std::optional<Instance<AxesController>> m_axes_controller;
		AxesProperties m_axes_properties;
	};

	template <typename AxisControllerConfig>
	inline MovementManager<AxisControllerConfig>::MovementManager(
		const AxesControllerCreator& axes_controller_ctor,
		const AxesProperties& axes_properties
	): m_axes_controller_ctor(axes_controller_ctor), m_axes_controller(std::nullopt), m_axes_properties(axes_properties) {
		if (!m_axes_controller_ctor) {
			throw std::invalid_argument("invalid axes_controller_ctor received");
		}
	}

	template <typename AxisControllerConfig>
	inline void MovementManager<AxisControllerConfig>::init(const AxisControllerConfig& axes_config) {
		m_axes_controller = m_axes_controller_ctor(axes_config);
	}

	template <typename AxisControllerConfig>
	inline void MovementManager<AxisControllerConfig>::linear_movement(const Vector<double>& destination, const double speed) {
		if (!m_axes_controller) {
			throw std::runtime_error("axes controller is not initialized");
		}
		auto movement = LinearMovement(
			destination,
			m_axes_properties,
			speed
		);
		m_axes_controller->get().enable();
		while (true) {
			const auto step = movement.next_step();
			if (!step) {
				break;
			}
			m_axes_controller->get().step(*step);
		}
		m_axes_controller->get().disable();
	}
	
	template <typename AxisControllerConfig>
	inline void MovementManager<AxisControllerConfig>::circular_movement(const Vector<double>& destination, const Vector<double>& rotation_center, const double speed) {
		throw std::runtime_error("not implemented");
	}
}

#endif // MOVEMENT_MANAGER_HPP