#ifndef	MOVEMENT_MANAGER_VECTOR_HPP
#define	MOVEMENT_MANAGER_VECTOR_HPP

#include <map>

#include "movement_manager_data.hpp"

namespace manager {
	template <typename T>
	class Vector {
	public:
		Vector(const T& x, const T& y, const T& z);
		Vector(const Vector&) = default;
		Vector& operator=(const Vector&) = default;
		~Vector() = default;

		T get(const Axis& axis) const;
		void set(const Axis& axis, const T& value);
	private:
		std::map<Axis, T> m_values;
	};

	template <typename T>
	inline Vector<T>::Vector(const T& x, const T& y, const T& z): m_values {
		{Axis::X, x},
		{Axis::Y, y},
		{Axis::Z, z}
	} {

	}

	template <typename T>
	inline T Vector<T>::get(const Axis& axis) const {
		return m_values.at(axis);
	}

	template <typename T>
	inline void Vector<T>::set(const Axis& axis, const T& value) {
		m_values[axis] = value;
	}

	template <typename T>
	inline double inner_product(const Vector<T>& one, const Vector<T>& other) {
		auto result = 0.0;
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			result += one.get(axis) * other.get(axis);
		}
		return result;
	}

	template <typename T>
	inline Vector<T> vector_product(const Vector<T>& one, const Vector<T>& other) {
		return Vector<T>(
			one.get(Axis::Y) * other.get(Axis::Z) - one.get(Axis::Z) * other.get(Axis::Y),
			- one.get(Axis::X) * other.get(Axis::Z) + one.get(Axis::Z) * other.get(Axis::X),
			one.get(Axis::X) * other.get(Axis::Y) - one.get(Axis::Y) * other.get(Axis::X)
		);
	}
}

#endif // MOVEMENT_MANAGER_VECTOR_HPP