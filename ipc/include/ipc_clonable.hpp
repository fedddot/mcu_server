#ifndef	IPC_CLONABLE_HPP
#define	IPC_CLONABLE_HPP

namespace ipc {
	template <typename T>
	class Clonable {
	public:
		virtual ~Clonable() noexcept = default;
		virtual T *clone() const = 0;
	};
}

#endif // IPC_CLONABLE_HPP