#ifndef	VECTOR_HPP
#define	VECTOR_HPP

#include <cmath>
#include <map>

namespace manager {
	template <typename T>
	class Vector {
	public:
		enum class Axis: int {X, Y, Z};
		Vector(const T& x, const T& y, const T& z);
		Vector(const Vector&) = default;
		Vector& operator=(const Vector&) = default;
		virtual ~Vector() noexcept = default;
		
		T projection(const Axis& axis) const;
		void set_projection(const Axis& axis, const T& value);
		Vector<T> scale(const float& factor) const;
		Vector<T> negate() const;
		float norm() const;
	private:
		std::map<Axis, T> m_projections;
	};

	template <typename T>
	inline Vector<T>::Vector(const T& x, const T& y, const T& z): m_projections({{Axis::X, x}, {Axis::Y, y}, {Axis::Z, z}}) {

	}

	template <typename T>
	inline T Vector<T>::projection(const Axis& axis) const {
		return m_projections.at(axis);
	}

	template <typename T>
	inline void Vector<T>::set_projection(const Axis& axis, const T& value) {
		m_projections[axis] = value;
	}

	template <typename T>
	inline Vector<T> Vector<T>::scale(const float& factor) const {
		return Vector<T>(
			static_cast<T>(factor * projection(Axis::X)),
			static_cast<T>(factor * projection(Axis::Y)),
			static_cast<T>(factor * projection(Axis::Z))
		);
	}

	template <typename T>
	inline float Vector<T>::norm() const {
		T normL2(0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			normL2 += projection(axis) * projection(axis);
		}
		return std::sqrt(normL2);
	}

	template <typename T>
	inline bool operator==(const Vector<T>& one, const Vector<T>& other) {
		using Axis = typename Vector<T>::Axis;
		return (one.projection(Axis::X) == other.projection(Axis::X))
			&& (one.projection(Axis::Y) == other.projection(Axis::Y))
			&& (one.projection(Axis::Z) == other.projection(Axis::Z));
	}

	template <typename T>
	inline Vector<T> operator+(const Vector<T>& one, const Vector<T>& other) {
		using Axis = typename Vector<T>::Axis;
		return Vector<T>(
			one.projection(Axis::X) + other.projection(Axis::X),
			one.projection(Axis::Y) + other.projection(Axis::Y),
			one.projection(Axis::Z) + other.projection(Axis::Z)
		);
	}

	template <typename T>
	inline Vector<T> operator-(const Vector<T>& one, const Vector<T>& other) {
		using Axis = typename Vector<T>::Axis;
		return Vector<T>(
			one.projection(Axis::X) - other.projection(Axis::X),
			one.projection(Axis::Y) - other.projection(Axis::Y),
			one.projection(Axis::Z) - other.projection(Axis::Z)
		);
	}
}

#endif // VECTOR_HPP