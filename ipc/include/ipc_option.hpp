#ifndef	IPC_OPTION_HPP
#define	IPC_OPTION_HPP

#include <memory>
#include <stdexcept>

namespace ipc {
	template <typename T>
	class Option {
	public:
		Option(T *instance);
		Option(const Option& other);
		Option& operator=(const Option& other);
		virtual ~Option() noexcept = default;
		bool some() const;
		T get() const;
	private:
		std::unique_ptr<T> m_instance;
	};

	template <typename T>
	inline Option<T>::Option(T *instance): m_instance(instance) {

	}

	template <typename T>
	inline Option<T>::Option(const Option& other): m_instance(nullptr) {
		if (other.m_instance) {
			m_instance = std::unique_ptr<T>(new T(*(other.m_instance)));
		}
	}

	template <typename T>
	inline Option<T>& Option<T>::operator=(const Option& other) {
		if (other.m_instance) {
			m_instance = std::unique_ptr<T>(new T(*(other.m_instance)));
		}
	}

	template <typename T>
	inline bool Option<T>::some() const {
		return nullptr != m_instance;
	}

	template <typename T>
	inline T Option<T>::get() const {
		if (!m_instance) {
			throw std::runtime_error("attempt to get an uninitialized option");
		}
		return *m_instance;
	}

}

#endif // IPC_OPTION_HPP