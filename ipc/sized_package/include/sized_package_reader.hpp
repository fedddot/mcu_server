#ifndef	SIZED_PACKAGE_READER_HPP
#define	SIZED_PACKAGE_READER_HPP

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <vector>

#include "request_reader.hpp"
#include "sized_package_infra.hpp"

namespace ipc {
	class SizedPackageReader: public RequestReader<std::vector<char>> {
	public:
		SizedPackageReader(
			std::vector<char> *buffer,
			const std::vector<char>& preamble
		);
		SizedPackageReader(const SizedPackageReader&) = delete;
		SizedPackageReader& operator=(const SizedPackageReader&) = delete;
		std::optional<std::vector<char>> read() override;
	private:
		std::vector<char> *m_buffer;
		const std::vector<char> m_preamble;
		static bool validate_preamble(std::vector<char> *buffer, const std::vector<char>& preamble);
	};

	inline SizedPackageReader::SizedPackageReader(
		std::vector<char> *buffer,
		const std::vector<char>& preamble
	): m_buffer(buffer), m_preamble(preamble) {
		if (!m_buffer) {
			throw std::invalid_argument("invalid buffer ptr received");
		}
		if (m_preamble.empty()) {
			throw std::invalid_argument("preamble must not be empty");
		}
	}

	inline std::optional<std::vector<char>> SizedPackageReader::read() {
		const auto preamble_size = m_preamble.size();
		if (!validate_preamble(m_buffer, m_preamble)) {
			return std::optional<std::vector<char>>();
		}
		const auto package_size = SizedPackageInfra::decode_size(
			std::vector<char>(
				m_buffer->begin() + preamble_size,
				m_buffer->begin() + preamble_size + sizeof(std::size_t)
			)
		);
		if (m_buffer->size() < preamble_size + sizeof(std::size_t) + package_size) {
			return std::optional<std::vector<char>>();
		}
		const auto package_data = std::vector<char>(
			m_buffer->begin() + preamble_size + sizeof(std::size_t),
			m_buffer->begin() + preamble_size + sizeof(std::size_t) + package_size
		);
		m_buffer->erase(
			m_buffer->begin(),
			m_buffer->begin() + preamble_size + sizeof(std::size_t) + package_size
		);
		return std::optional<std::vector<char>>(package_data);
	}

	inline bool SizedPackageReader::validate_preamble(std::vector<char> *buffer, const std::vector<char>& preamble) {
		if (!buffer) {
			throw std::invalid_argument("invalid buffer ptr received");
		}
		const auto preamble_size = preamble.size();
		while (buffer->size() >= preamble_size + sizeof(std::size_t)) {
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