#ifndef	PACKAGE_WRITER_HPP
#define	PACKAGE_WRITER_HPP

#include <stdexcept>
#include <vector>

#include "data_writer.hpp"
#include "ipc_types.hpp"

namespace ipc {
	class PackageWriter: public DataWriter<Package> {
	public:
		using Preamble = std::vector<char>;
		PackageWriter(const Preamble& preamble, DataWriter<RawData> *raw_data_writer_ptr);
		PackageWriter(const PackageWriter&) = delete;
		PackageWriter& operator=(const PackageWriter&) = delete;

		void write(const Package& data) const override;
	private:
		const Preamble m_preamble;
		DataWriter<RawData> *m_raw_data_writer_ptr;

		static RawData serialize_size(const PackageSize& size);
	};

	inline PackageWriter::PackageWriter(const Preamble& preamble, DataWriter<RawData> *raw_data_writer_ptr): m_preamble(preamble), m_raw_data_writer_ptr(raw_data_writer_ptr) {
		if (!m_raw_data_writer_ptr) {
			throw std::invalid_argument("invalid raw data writer ptr received");
		}
	}

	inline void PackageWriter::write(const Package& data) const {
		RawData raw_data;
		const auto data_size = static_cast<PackageSize>(data.size());
		raw_data.reserve(m_preamble.size() + sizeof(PackageSize) + data_size);
		raw_data.insert(raw_data.end(), m_preamble.begin(), m_preamble.end());
		const auto size_serialized = serialize_size(data_size);
		raw_data.insert(raw_data.end(), size_serialized.begin(), size_serialized.end());
		raw_data.insert(raw_data.end(), data.begin(), data.end());
		m_raw_data_writer_ptr->write(raw_data);
	}

	inline RawData PackageWriter::serialize_size(const PackageSize& size) {
		enum: int {BITS_IN_BYTE = 8};
		RawData buff;
		buff.reserve(sizeof(PackageSize));
		for (auto byte_number = 0; byte_number < sizeof(PackageSize); ++byte_number) {
			const auto ch = static_cast<char>((size >> (byte_number * BITS_IN_BYTE)) & 0xFF);
			buff.push_back(ch);
		}
		return buff;
	}
}

#endif // PACKAGE_WRITER_HPP