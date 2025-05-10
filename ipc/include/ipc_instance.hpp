#ifndef	IPC_INSTANCE_HPP
#define	IPC_INSTANCE_HPP

#include <memory>
#include <stdexcept>

namespace ipc {
	template <typename T>
	class Result {
	public:
		Result(T *instance_ptr);
		Result(const Result&) = default;
		virtual ~Result() noexcept = default;
		const T& get() const;
		T& get();
	private:
		std::shared_ptr<T> m_instance_ptr;
	};

	template <typename T>
	inline Result<T>::Result(T *instance_ptr): m_instance_ptr(instance_ptr) {
		if (!m_instance_ptr) {
			throw std::invalid_argument("invalid instance ptr received");
		}
	}

	template <typename T>
	inline const T& Result<T>::get() const {
		return std::ref(*m_instance_ptr);
	}

	template <typename T>
	inline T& Result<T>::get() {
		return std::ref(*m_instance_ptr);
	}
}

#endif // IPC_INSTANCE_HPP