#ifndef	IPC_QUEUE_HPP
#define	IPC_QUEUE_HPP

#include <cstddef>

namespace ipc {
	template <typename T>
	class IpcQueue {
	public:
		virtual ~IpcQueue() noexcept = default;
		virtual void enqueue(const T& elem) = 0;
		virtual T dequeue() = 0;
		virtual const T& inspect(const std::size_t index) const = 0;
		virtual std::size_t size() const = 0;
		virtual std::size_t capacity() const = 0;
		virtual void clear() = 0;
	};
}

#endif // IPC_QUEUE_HPP