#ifndef	RING_DATA_BUFFER_HPP
#define	RING_DATA_BUFFER_HPP

#include <cstddef>
#include <cstdint>

#include "data_buffer.hpp"

namespace tools {
	class RingDataBuffer: public DataBuffer {
	public:
		RingDataBuffer() = default;
		RingDataBuffer(const RingDataBuffer&) = default;
		RingDataBuffer& operator=(const RingDataBuffer&) = default;
		
		void read_bytes(std::uint8_t *destination, const std::size_t size) override;
		void write_bytes(const std::uint8_t *source, const std::size_t size) override;
	};
}

#endif // RING_DATA_BUFFER_HPP