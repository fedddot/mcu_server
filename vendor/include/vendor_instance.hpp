#ifndef	VENDOR_INSTANCE_HPP
#define	VENDOR_INSTANCE_HPP

#include <memory>
#include <stdexcept>

namespace vendor {
	template <typename T>
	class Instance {
	public:
		Instance(T *instance_ptr);
		Instance(const Instance&) = delete;
		Instance& operator=(const Instance&) = delete;

		Instance(Instance&&) = default; // Default move constructor
		Instance& operator=(Instance&&) = default; // Default move assignment operator

		virtual ~Instance() noexcept = default;
		const T& get() const;
		T& get();
	private:
		std::unique_ptr<T> m_instance_ptr;
	};

	template <typename T>
	inline Instance<T>::Instance(T *instance_ptr): m_instance_ptr(instance_ptr) {
		if (!m_instance_ptr) {
			throw std::invalid_argument("invalid instance ptr received");
		}
	}

	template <typename T>
	inline const T& Instance<T>::get() const {
		return std::ref(*m_instance_ptr);
	}

	template <typename T>
	inline T& Instance<T>::get() {
		return std::ref(*m_instance_ptr);
	}
}

#endif // VENDOR_INSTANCE_HPP