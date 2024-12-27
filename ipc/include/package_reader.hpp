#ifndef	PACKAGE_READER_HPP
#define	PACKAGE_READER_HPP

#include <stdexcept>
#include <vector>

#include "data_reader.hpp"
#include "ipc_types.hpp"

namespace ipc {
	class PackageReader: public DataReader<Package> {
	public:
		using Preamble = std::vector<char>;
		PackageReader(const Preamble& preamble);
		PackageReader(const PackageReader&) = delete;
		PackageReader& operator=(const PackageReader&) = delete;

		bool readable() const override;
		Package read() const override;
		void feed(const char ch);
	private:
		const Preamble m_preamble;
		const std::size_t m_preamble_size;

		mutable RawData m_buffer;

		void reset_buffer();

		static PackageSize parse_size(const RawData& raw_data);
	};

	inline PackageReader::PackageReader(const Preamble& preamble): m_preamble(preamble), m_preamble_size(preamble.size()) {
		reset_buffer();
	}

	inline bool PackageReader::readable() const {
		if (m_buffer.size() < m_preamble_size + sizeof(PackageSize)) {
			return false;
		}
		const auto received_preamble = Preamble(m_buffer.begin(), m_buffer.begin() + m_preamble_size);
		if (received_preamble != m_preamble) {
			m_buffer.erase(m_buffer.begin());
			return false;
		}
		const auto pckg_size_data = RawData(
			m_buffer.begin() + m_preamble_size,
			m_buffer.begin() + m_preamble_size + sizeof(PackageSize)
		);
		const auto package_size = parse_size(pckg_size_data);
		return m_buffer.size() >= m_preamble_size + sizeof(PackageSize) + package_size;
	}

	inline Package PackageReader::read() const {
		if (!readable()) {
			throw std::runtime_error("package is not readable");
		}
		const auto package_data_start_pos = m_preamble_size + sizeof(PackageSize);
		const auto pckg_size_data = RawData(
			m_buffer.begin() + m_preamble_size,
			m_buffer.begin() + m_preamble_size + sizeof(PackageSize)
		);
		const auto package_size = parse_size(pckg_size_data);
		const auto package_data_end_pos = package_data_start_pos + package_size;
		const auto package = Package(
			m_buffer.begin() + package_data_start_pos,
			m_buffer.begin() + package_data_end_pos
		);
		m_buffer.erase(m_buffer.begin(), m_buffer.begin() + package_data_end_pos);
		return package;
	}

	inline void PackageReader::feed(const char ch) {
		enum: int {RESIZE_ADDITION = 10};
		if (m_buffer.size() == m_buffer.capacity()) {
			m_buffer.reserve(m_buffer.size() + RESIZE_ADDITION);
		}
		m_buffer.push_back(ch);
	}

	inline void PackageReader::reset_buffer() {
		m_buffer.clear();
		m_buffer.reserve(m_preamble.size() + sizeof(PackageSize));
	}

	inline PackageSize PackageReader::parse_size(const RawData& raw_data) {
		enum: int {BITS_IN_BYTE = 8};
		if (sizeof(PackageSize) != raw_data.size()) {
			throw std::invalid_argument("invalid raw data received");
		}
		PackageSize result(0);
		for (auto byte_number = 0; byte_number < sizeof(PackageSize); ++byte_number) {
			const auto chunk = raw_data[sizeof(PackageSize) - 1 - byte_number];
			result <<= BITS_IN_BYTE;
			result |= chunk;
		}
		return result;
	}
}

#endif // PACKAGE_READER_HPP