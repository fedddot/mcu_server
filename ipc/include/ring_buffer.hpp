#ifndef	RING_BUFFER_HPP
#define	RING_BUFFER_HPP

#include <array>
#include <cstddef>
#include <stdexcept>

namespace ipc {
	template <typename T, std::size_t N>
	class RingBuffer {
	public:
		RingBuffer();
		RingBuffer(const RingBuffer&) = default;
		RingBuffer& operator=(const RingBuffer&) = default;
		virtual ~RingBuffer() noexcept = default;
		
		void push_back(const T& element);
		T pop_front();
		std::size_t data_size() const;
		const T *raw_data() const;
	private:
		std::array<T, N> m_buffer;
		std::size_t m_read_index, m_write_index;
	};

	template <typename T, std::size_t N>
	inline RingBuffer<T, N>::RingBuffer(): m_read_index(0UL), m_write_index(0UL) {

	}

	template <typename T, std::size_t N>
	inline void RingBuffer<T, N>::push_back(const T& element) {
		throw std::runtime_error("not implemented");
	}

	template <typename T, std::size_t N>
	inline T RingBuffer<T, N>::pop_front() {
		throw std::runtime_error("not implemented");
	}

	template <typename T, std::size_t N>
	inline std::size_t RingBuffer<T, N>::data_size() const {
		throw std::runtime_error("not implemented");
	}

	template <typename T, std::size_t N>
	const T *RingBuffer<T, N>::raw_data() const {
		return m_buffer.data();
	}
}

#endif // RING_BUFFER_HPP