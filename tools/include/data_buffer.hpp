#ifndef	DATA_BUFFER_HPP
#define	DATA_BUFFER_HPP

#include <cstddef>
#include <cstdint>

namespace tools {
	class DataBuffer {
	public:
		virtual ~DataBuffer() noexcept = default;
		virtual void read_bytes(std::uint8_t *destination, const std::size_t size) = 0;
		virtual void write_bytes(const std::uint8_t *source, const std::size_t size) = 0;
		
	};
}

#endif // DATA_BUFFER_HPP