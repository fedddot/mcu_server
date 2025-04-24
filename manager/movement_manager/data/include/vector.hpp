#ifndef	VECTOR_HPP
#define	VECTOR_HPP

namespace manager {
	struct Vector {
		Vector() = default;
		Vector(double x, double y, double z);
		Vector(const Vector&) = default;
		Vector& operator=(const Vector&) = default;
		~Vector() = default;

		bool zero_vector() const {
			return (x == 0.0 && y == 0.0 && z == 0.0);
		}

		double x;
		double y;
		double z;
	};

	inline Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {
	}

	inline bool operator==(const Vector& lhs, const Vector& rhs) {
		return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
	}

	inline Vector operator+(const Vector& lhs, const Vector& rhs) {
		return Vector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	inline Vector operator-(const Vector& lhs, const Vector& rhs) {
		return Vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	inline Vector operator-(const Vector& lhs) {
		return Vector(-lhs.x, -lhs.y, -lhs.z);
	}
}

#endif // VECTOR_HPP