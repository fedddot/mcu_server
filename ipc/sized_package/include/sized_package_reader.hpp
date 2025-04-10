#ifndef	SIZED_PACKAGE_READER_HPP
#define	SIZED_PACKAGE_READER_HPP

#include <cstddef>
#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

#include "request_reader.hpp"

namespace ipc {
	class SizedPackageReader: public RequestReader<std::vector<char>> {
	public:
		SizedPackageReader(const std::vector<char>& preamble);
		SizedPackageReader(const SizedPackageReader&) = delete;
		SizedPackageReader& operator=(const SizedPackageReader&) = delete;
		std::optional<std::vector<char>> read() override;
		void feed(const char ch);
	private:
		enum class State: int {
			RECEIVING_PREAMBLE,
			RECEIVING_SIZE,
			RECEIVING_DATA,
		};
		const std::vector<char> m_preamble;
		std::vector<char> m_buffer;
		State m_state;
		std::size_t m_msg_size;

		void reset_reader();
	};

	inline SizedPackageReader::SizedPackageReader(const std::vector<char>& preamble):
		m_preamble(preamble), m_state(State::RECEIVING_PREAMBLE), m_msg_size(0UL) {

	}

	inline std::optional<std::vector<char>> SizedPackageReader::read() {
		throw std::runtime_error("NOT IMPL");
	}

	inline void SizedPackageReader::feed(const char ch) {
		throw std::runtime_error("NOT IMPL");
	}

	inline void SizedPackageReader::reset_reader() {
		throw std::runtime_error("NOT IMPL");
	}
}

#endif // SIZED_PACKAGE_READER_HPP