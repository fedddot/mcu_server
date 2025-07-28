#ifndef	DATA_BUFFER_HPP
#define	DATA_BUFFER_HPP

#include <cstddef>

namespace host_tools {
	template <typename T>
	class DataBuffer {
	public:
		virtual ~DataBuffer() noexcept = default;
		virtual void push_back(const T& elem) = 0;
		virtual T pop_first() = 0;
		virtual const T& get(const std::size_t index) const = 0;
		virtual std::size_t size() const = 0;
		virtual std::size_t capacity() const = 0;
	};
}

#endif // DATA_BUFFER_HPP