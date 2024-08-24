#ifndef	ZLIB_COMPRESSOR_HPP
#define	ZLIB_COMPRESSOR_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include "zlib.h"

namespace mcu_server_utl {
	class ZlibCompressor {
	public:
		ZlibCompressor(const int compression_level);
		ZlibCompressor(const ZlibCompressor& other) = default;
		ZlibCompressor& operator=(const ZlibCompressor& other) = default;

		std::vector<char> transform(const std::vector<char>& data) const;
	private:
		int m_compression_level;
	};

	inline ZlibCompressor::ZlibCompressor(const int compression_level): m_compression_level(compression_level) {
		if ((-1 > m_compression_level) || (9 < m_compression_level)) {
			throw std::invalid_argument("invalid compression level received");
		}
	}

	inline std::vector<char> ZlibCompressor::transform(const std::vector<char>& data) const {
		const auto data_size(data.size());
		std::unique_ptr<Bytef[]> data_buff(new Bytef[data_size]);
		if (!std::copy(data.begin(), data.end(), data_buff.get())) {
			throw std::runtime_error("failed to copy data");
		}
		const auto result_buff_size(compressBound(data_size));
		std::unique_ptr<Bytef[]> result_buff(new Bytef[result_buff_size]);
		uLong actual_result_size(result_buff_size);
		if (Z_OK != compress(result_buff.get(), &actual_result_size, data_buff.get(), data_size)) {
			throw std::runtime_error("failed to compress data");
		}
		return std::vector<char>(result_buff.get(), result_buff.get() + actual_result_size);
	}
}

#endif // ZLIB_COMPRESSOR_HPP