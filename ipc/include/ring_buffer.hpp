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
	private:
		std::array<T, N> m_buffer;
		std::size_t m_read_index, m_write_index, m_size;

		static std::size_t next_index(const std::size_t& current_index);
	};

	template <typename T, std::size_t N>
	inline RingBuffer<T, N>::RingBuffer(): m_read_index(0UL), m_write_index(0UL), m_size(0UL) {

	}

	template <typename T, std::size_t N>
	inline void RingBuffer<T, N>::push_back(const T& element) {
		if (m_size + 1UL > N) {
			throw std::runtime_error("attempt to push to full buffer");
		}
		const auto current_write_index = m_write_index;
		m_write_index = next_index(current_write_index);
		++m_size;
		m_buffer[current_write_index] = element;
	}

	template <typename T, std::size_t N>
	inline T RingBuffer<T, N>::pop_front() {
		if (!m_size) {
			throw std::runtime_error("attempt to pop from empty buffer");
		}
		const auto current_read_index = m_read_index;
		m_read_index = next_index(current_read_index);
		--m_size;
		return m_buffer[current_read_index];
	}

	template <typename T, std::size_t N>
	inline std::size_t RingBuffer<T, N>::data_size() const {
		return m_size;
	}

	template <typename T, std::size_t N>
	std::size_t RingBuffer<T, N>::next_index(const std::size_t& current_index) {
		if (current_index + 1UL >= N) {
			return 0;
		}
		return current_index + 1UL;
	}
}

#endif // RING_BUFFER_HPP