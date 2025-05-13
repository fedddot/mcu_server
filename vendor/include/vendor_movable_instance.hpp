#ifndef	MOVABLE_INSTANCE_HPP
#define	MOVABLE_INSTANCE_HPP

#include <memory>
#include <stdexcept>

namespace vendor {
	template <typename T>
	class MovableInstance {
	public:
		MovableInstance(T *instance_ptr);
		MovableInstance(const MovableInstance&) = delete;
		MovableInstance& operator=(const MovableInstance&) = delete;

		MovableInstance(MovableInstance&&) = default; // Default move constructor
		MovableInstance& operator=(MovableInstance&&) = default; // Default move assignment operator

		virtual ~MovableInstance() noexcept = default;
		const T& get() const;
		T& get();
	private:
		std::unique_ptr<T> m_instance_ptr;
	};

	template <typename T>
	inline MovableInstance<T>::MovableInstance(T *instance_ptr): m_instance_ptr(instance_ptr) {
		if (!m_instance_ptr) {
			throw std::invalid_argument("invalid instance ptr received");
		}
	}

	template <typename T>
	inline const T& MovableInstance<T>::get() const {
		return std::ref(*m_instance_ptr);
	}

	template <typename T>
	inline T& MovableInstance<T>::get() {
		return std::ref(*m_instance_ptr);
	}
}

#endif // MOVABLE_INSTANCE_HPP