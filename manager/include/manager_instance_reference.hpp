#ifndef	MANAGER_INSTANCE_REFERENCE_HPP
#define	MANAGER_INSTANCE_REFERENCE_HPP

#include <stdexcept>

namespace manager {
	template <typename T>
	class InstanceReference {
	public:
		InstanceReference(T *instance_ptr);
		InstanceReference(const InstanceReference&) = default;
		virtual ~InstanceReference() noexcept = default;
		const T& get() const;
		T& get();
	private:
		T *m_instance_ptr;
	};

	template <typename T>
	inline InstanceReference<T>::InstanceReference(T *instance_ptr): m_instance_ptr(instance_ptr) {
		if (!m_instance_ptr) {
			throw std::invalid_argument("invalid instance ptr received");
		}
	}

	template <typename T>
	inline const T& InstanceReference<T>::get() const {
		return std::ref(*m_instance_ptr);
	}

	template <typename T>
	inline T& InstanceReference<T>::get() {
		return std::ref(*m_instance_ptr);
	}
}

#endif // MANAGER_INSTANCE_REFERENCE_HPP