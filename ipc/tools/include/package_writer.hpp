#ifndef	RAW_DATA_PACKAGE_WRITER_HPP
#define	RAW_DATA_PACKAGE_WRITER_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <vector>

#include "ipc_data_writer.hpp"

namespace ipc {
	template <std::size_t HSIZE>
	class PackageWriter: public DataWriter<std::vector<std::uint8_t>> {
	public:
		using HeaderGenerator = std::function<std::vector<std::uint8_t>(const std::vector<std::uint8_t>&, const std::size_t&)>;
		using RawDataWriter = std::function<void(const std::vector<std::uint8_t>&)>;
		PackageWriter(
			const HeaderGenerator& header_generator,
			const RawDataWriter& raw_data_writer
		): m_header_generator(header_generator), m_raw_data_writer(raw_data_writer) {
			if (!m_header_generator || !m_raw_data_writer) {
				throw std::invalid_argument("invalid args in package writer received");
			}
		}
		PackageWriter(const PackageWriter&) = default;
		PackageWriter& operator=(const PackageWriter&) = default;
		void write(const std::vector<std::uint8_t>& payload) const override {
			const auto header = m_header_generator(payload, HSIZE);
			if (header.size() != HSIZE) {
				throw std::invalid_argument("generated header has invalid size");
			}
			m_raw_data_writer(header);
			m_raw_data_writer(payload);
		}
	private:
		HeaderGenerator m_header_generator;
		RawDataWriter m_raw_data_writer;
	};
}

#endif // RAW_DATA_PACKAGE_WRITER_HPP