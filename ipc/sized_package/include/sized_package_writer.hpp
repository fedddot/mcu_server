#ifndef	SIZED_PACKAGE_WRITER_HPP
#define	SIZED_PACKAGE_WRITER_HPP

#include <functional>
#include <vector>

#include "response_writer.hpp"
#include "sized_package_common.hpp"

namespace ipc {
	class SizedPackageWriter: public IpcDataWriter<std::vector<char>> {
	public:
		using RawDataWriter = std::function<void(const std::vector<char>&)>;
		SizedPackageWriter(
			const RawDataWriter& raw_data_writer,
			const std::vector<char>& preamble,
			const std::size_t& raw_package_data_size_length
		);
		SizedPackageWriter(const SizedPackageWriter&) = default;
		SizedPackageWriter& operator=(const SizedPackageWriter&) = default;
		void write(const std::vector<char>& response) const override;
	private:
		RawDataWriter m_raw_data_writer;
		std::vector<char> m_preamble;
		DefaultPackageSizeSerializer m_size_serializer;
	};

	inline SizedPackageWriter::SizedPackageWriter(
		const RawDataWriter& raw_data_writer,
		const std::vector<char>& preamble,
		const std::size_t& raw_package_data_size_length
	): m_raw_data_writer(raw_data_writer), m_preamble(preamble), m_size_serializer(raw_package_data_size_length) {
		if (!m_raw_data_writer) {
			throw std::invalid_argument("invalid raw data writer received");
		}
		if (m_preamble.empty()) {
			throw std::invalid_argument("preamble must not be empty");
		}
	}

	inline void SizedPackageWriter::write(const std::vector<char>& response) const {
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

#endif // SIZED_PACKAGE_WRITER_HPP