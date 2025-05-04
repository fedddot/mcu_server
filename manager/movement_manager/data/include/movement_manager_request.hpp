#ifndef	MOVEMENT_MANAGER_REQUEST_HPP
#define	MOVEMENT_MANAGER_REQUEST_HPP

#include <optional>
#include <stdexcept>

#include "movement_manager_vector.hpp"

namespace manager {
	class MovementManagerRequest {
	public:
		enum class MovementType: int {
			LINEAR,
			ROTATIONAL,
		};		
		struct LinearMovementData {
			Vector<double> destination;
			double speed;
		};
		struct RotationalMovementData {
			Vector<double> rotation_center;
			double angle;
			double speed;
		};
		MovementManagerRequest(const LinearMovementData& linear_movement_data);
		MovementManagerRequest(const RotationalMovementData& rotational_movement_data);
		MovementManagerRequest(const MovementManagerRequest&) = default;
		MovementManagerRequest& operator=(const MovementManagerRequest&) = default;
		~MovementManagerRequest() noexcept = default;

		MovementType get_movement_type() const;

		template <typename T>
		T get_movement_data() const;
	private:
		MovementType m_type;
		std::optional<LinearMovementData> m_linear_movement_data;
		std::optional<RotationalMovementData> m_rotational_movement_data;
	};

	inline MovementManagerRequest::MovementManagerRequest(const LinearMovementData& linear_movement_data): m_type(MovementType::LINEAR), m_linear_movement_data(linear_movement_data), m_rotational_movement_data(std::nullopt) {

	}

	inline MovementManagerRequest::MovementManagerRequest(const RotationalMovementData& rotational_movement_data): m_type(MovementType::ROTATIONAL), m_linear_movement_data(std::nullopt), m_rotational_movement_data(rotational_movement_data) {

	}

	inline MovementManagerRequest::MovementType MovementManagerRequest::get_movement_type() const {
		return m_type;
	}

	template <>
	inline MovementManagerRequest::LinearMovementData MovementManagerRequest::get_movement_data<MovementManagerRequest::LinearMovementData>() const {
		if (!m_linear_movement_data) {
			throw std::runtime_error("attempt to get linear movement data from a non-linear movement request");
		}
		return *m_linear_movement_data;
	}

	template <>
	inline MovementManagerRequest::RotationalMovementData MovementManagerRequest::get_movement_data<MovementManagerRequest::RotationalMovementData>() const {
		if (!m_rotational_movement_data) {
			throw std::runtime_error("attempt to get rotational movement data from a non-rotational movement request");
		}
		return *m_rotational_movement_data;
	}
}

#endif // MOVEMENT_MANAGER_REQUEST_HPP