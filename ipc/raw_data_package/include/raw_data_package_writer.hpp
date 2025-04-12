#ifndef	RAW_DATA_PACKAGE_WRITER_HPP
#define	RAW_DATA_PACKAGE_WRITER_HPP

#include <functional>
#include <vector>

#include "ipc_data_writer.hpp"
#include "raw_data_package_descriptor.hpp"

namespace ipc {
	class RawDataPackageWriter: public IpcDataWriter<std::vector<char>> {
	public:
		using RawDataWriter = std::function<void(const std::vector<char>&)>;
		RawDataPackageWriter(
			const RawDataWriter& raw_data_writer,
			const std::vector<char>& preamble,
			const std::size_t& encoded_package_size_len
		);
		RawDataPackageWriter(const RawDataPackageWriter&) = default;
		RawDataPackageWriter& operator=(const RawDataPackageWriter&) = default;
		void write(const std::vector<char>& response) const override;
	private:
		RawDataWriter m_raw_data_writer;
		std::vector<char> m_preamble;
		DefaultPackageSizeSerializer m_size_serializer;
	};

	inline RawDataPackageWriter::RawDataPackageWriter(
		const RawDataWriter& raw_data_writer,
		const std::vector<char>& preamble,
		const std::size_t& encoded_package_size_len
	): m_raw_data_writer(raw_data_writer), m_preamble(preamble), m_size_serializer(encoded_package_size_len) {
		if (!m_raw_data_writer) {
			throw std::invalid_argument("invalid raw data writer received");
		}
		if (m_preamble.empty()) {
			throw std::invalid_argument("preamble must not be empty");
		}
	}

	inline void RawDataPackageWriter::write(const std::vector<char>& response) const {
		const auto encoded_size = m_size_serializer.transform(response.size());
		auto raw_data = std::vector<char>();
		raw_data.insert(
			raw_data.end(),
			m_preamble.begin(),
			m_preamble.end()
		);
		raw_data.insert(
			raw_data.end(),
			encoded_size.begin(),
			encoded_size.end()
		);
		raw_data.insert(
			raw_data.end(),
			response.begin(),
			response.end()
		);
		m_raw_data_writer(raw_data);
	}
}

#endif // RAW_DATA_PACKAGE_WRITER_HPP