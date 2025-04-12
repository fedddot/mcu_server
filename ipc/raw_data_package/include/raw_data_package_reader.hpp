#ifndef	SIZED_PACKAGE_READER_HPP
#define	SIZED_PACKAGE_READER_HPP

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <vector>

#include "ipc_data_reader.hpp"
#include "raw_data_package_common.hpp"

namespace ipc {
	class SizedPackageReader: public IpcDataReader<std::vector<char>> {
	public:
		SizedPackageReader(
			std::vector<char> *buffer,
			const std::vector<char>& preamble,
			const std::size_t& raw_package_data_size_length
		);
		SizedPackageReader(const SizedPackageReader&) = default;
		SizedPackageReader& operator=(const SizedPackageReader&) = default;
		std::optional<std::vector<char>> read() override;
	private:
		std::vector<char> *m_buffer;
		std::vector<char> m_preamble;
		DefaultPackageSizeParser m_size_parser;
		
		bool validate_preamble(std::vector<char> *buffer, const std::vector<char>& preamble);
	};

	inline SizedPackageReader::SizedPackageReader(
		std::vector<char> *buffer,
		const std::vector<char>& preamble,
		const std::size_t& raw_package_data_size_length
	):
		m_buffer(buffer), m_preamble(preamble),
		m_size_parser(raw_package_data_size_length) {
		
		if (!m_buffer) {
			throw std::invalid_argument("invalid buffer ptr received");
		}
		if (m_preamble.empty()) {
			throw std::invalid_argument("preamble must not be empty");
		}
	}

	inline std::optional<std::vector<char>> SizedPackageReader::read() {
		const auto preamble_size = m_preamble.size();
		const auto encoded_size_len = m_size_parser.raw_data_size();
		if (!validate_preamble(m_buffer, m_preamble)) {
			return std::optional<std::vector<char>>();
		}
		const auto package_size = m_size_parser.transform(
			std::vector<char>(
				m_buffer->begin() + preamble_size,
				m_buffer->begin() + preamble_size + encoded_size_len
			)
		);
		if (m_buffer->size() < preamble_size + encoded_size_len + package_size) {
			return std::optional<std::vector<char>>();
		}
		const auto package_data = std::vector<char>(
			m_buffer->begin() + preamble_size + encoded_size_len,
			m_buffer->begin() + preamble_size + encoded_size_len + package_size
		);
		m_buffer->erase(
			m_buffer->begin(),
			m_buffer->begin() + preamble_size + encoded_size_len + package_size
		);
		return std::optional<std::vector<char>>(package_data);
	}

	inline bool SizedPackageReader::validate_preamble(std::vector<char> *buffer, const std::vector<char>& preamble) {
		if (!buffer) {
			throw std::invalid_argument("invalid buffer ptr received");
		}
		const auto preamble_size = preamble.size();
		const auto encoded_size_len = m_size_parser.raw_data_size();
		while (buffer->size() >= preamble_size + encoded_size_len) {
			const auto incoming_preamble = std::vector<char>(
				buffer->begin(),
				buffer->begin() + preamble_size
			);
			if (preamble == incoming_preamble) {
				return true;
			}
			buffer->erase(buffer->begin());
		}
		return false;
	}
}

#endif // SIZED_PACKAGE_READER_HPP