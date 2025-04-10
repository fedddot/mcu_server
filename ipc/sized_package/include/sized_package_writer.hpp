#ifndef	SIZED_PACKAGE_WRITER_HPP
#define	SIZED_PACKAGE_WRITER_HPP

#include <functional>
#include <vector>

#include "response_writer.hpp"
#include "sized_package_infra.hpp"

namespace ipc {
	class SizedPackageWriter: public ResponseWriter<std::vector<char>> {
	public:
		using RawDataWriter = std::function<void(const std::vector<char>&)>;
		SizedPackageWriter(
			const RawDataWriter& raw_data_writer,
			const std::vector<char>& preamble
		);
		SizedPackageWriter(const SizedPackageWriter&) = delete;
		SizedPackageWriter& operator=(const SizedPackageWriter&) = delete;
		void write(const std::vector<char>& response) const override;
	private:
		const RawDataWriter m_raw_data_writer;
		const std::vector<char> m_preamble;
	};

	inline SizedPackageWriter::SizedPackageWriter(
		const RawDataWriter& raw_data_writer,
		const std::vector<char>& preamble
	): m_raw_data_writer(raw_data_writer), m_preamble(preamble) {
		if (!m_raw_data_writer) {
			throw std::invalid_argument("invalid raw data writer received");
		}
		if (m_preamble.empty()) {
			throw std::invalid_argument("preamble must not be empty");
		}
	}

	inline void SizedPackageWriter::write(const std::vector<char>& response) const {
		const auto encoded_size = SizedPackageInfra::encode_size(response.size());
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