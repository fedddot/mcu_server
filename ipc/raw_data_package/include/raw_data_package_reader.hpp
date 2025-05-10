#ifndef	RAW_DATA_PACKAGE_READER_HPP
#define	RAW_DATA_PACKAGE_READER_HPP

#include <cstddef>
#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
#include "raw_data_package_descriptor.hpp"

namespace ipc {
	class RawDataPackageReader: public IpcDataReader<RawData> {
	public:
		using PackageSizeParser = std::function<std::size_t(const RawDataPackageDescriptor&, const RawData&)>;
		RawDataPackageReader(
			RawData *buffer,
			const RawDataPackageDescriptor& descriptor,
			const PackageSizeParser& size_parser
		);
		RawDataPackageReader(const RawDataPackageReader&) = default;
		RawDataPackageReader& operator=(const RawDataPackageReader&) = default;
		std::optional<Instance<RawData>> read() override;
	private:
		RawData *m_buffer;
		RawDataPackageDescriptor m_descriptor;
		PackageSizeParser m_size_parser;
		
		bool validate_preamble();
	};

	inline RawDataPackageReader::RawDataPackageReader(
		RawData *buffer,
		const RawDataPackageDescriptor& descriptor,
		const PackageSizeParser& size_parser
	): m_buffer(buffer), m_descriptor(descriptor), m_size_parser(size_parser) {
		if (!m_buffer) {
			throw std::invalid_argument("invalid buffer ptr received");
		}
		if (!m_size_parser) {
			throw std::invalid_argument("invalid size parser received");
		}
	}

	inline std::optional<Instance<RawData>> RawDataPackageReader::read() {
		const auto preamble_size = m_descriptor.preamble().size();
		const auto encoded_size_len = m_descriptor.encoded_size_length();
		if (!validate_preamble()) {
			return std::nullopt;
		}
		const auto package_size = m_size_parser(
			m_descriptor,
			RawData(
				m_buffer->begin() + preamble_size,
				m_buffer->begin() + preamble_size + encoded_size_len
			)
		);
		if (m_buffer->size() < preamble_size + encoded_size_len + package_size) {
			return std::nullopt;
		}
		const auto package_data = RawData(
			m_buffer->begin() + preamble_size + encoded_size_len,
			m_buffer->begin() + preamble_size + encoded_size_len + package_size
		);
		m_buffer->erase(
			m_buffer->begin(),
			m_buffer->begin() + preamble_size + encoded_size_len + package_size
		);
		return Instance<RawData>(new RawData(package_data));
	}

	inline bool RawDataPackageReader::validate_preamble() {
		const auto preamble_size = m_descriptor.preamble().size();
		const auto encoded_size_len = m_descriptor.encoded_size_length();
		while (m_buffer->size() >= preamble_size + encoded_size_len) {
			const auto incoming_preamble = RawData(
				m_buffer->begin(),
				m_buffer->begin() + preamble_size
			);
			if (m_descriptor.preamble() == incoming_preamble) {
				return true;
			}
			m_buffer->erase(m_buffer->begin());
		}
		return false;
	}
}

#endif // RAW_DATA_PACKAGE_READER_HPP