#ifndef	API_REQUEST_READER_HPP
#define	API_REQUEST_READER_HPP

#include <climits>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

#include "data_buffer.hpp"

namespace host_tools {
	/// @brief A class to read API requests from a data buffer.
	/// @tparam ApiRequest The type of the API request to be read.
	/// @tparam L The size of the length field in bytes.
	/// @details package format: <size: L bytes, LSB first><payload: size bytes>
	template <typename ApiRequest, std::size_t L>
	class ApiRequestReader {
	public:
		/// @brief A type alias for the raw data buffer used to read API requests.
		using RawDataBuffer = DataBuffer<std::uint8_t>;
		/// @brief A function type for parsing the API request from the raw data.
		using ApiRequestParser = std::function<ApiRequest(const std::vector<std::uint8_t>&)>;
		/// @brief Constructs an ApiRequestReader with the given buffer and parser.
		/// @param buffer_ptr Pointer to the raw data buffer from which API requests will be read.
		/// @param parser Function to parse the API request from the raw data.
		/// @throws std::invalid_argument if the buffer pointer or parser is null.
		/// @details The buffer lifetime must exceed the ApiRequestReader lifetime.
		ApiRequestReader(RawDataBuffer *buffer_ptr, const ApiRequestParser& parser): m_buffer_ptr(buffer_ptr), m_parser(parser) {
			if (!m_buffer_ptr || !m_parser) {
				throw std::invalid_argument("invalid api request reader arguments");
			}
		}
		ApiRequestReader(const ApiRequestReader&) = default;
		ApiRequestReader& operator=(const ApiRequestReader&) = default;
		virtual ~ApiRequestReader() noexcept = default;

		/// @brief Reads an API request from the buffer.
		/// @return An optional containing the parsed API request if enough data is available, otherwise std::nullopt.
		std::optional<ApiRequest> operator()(void) {
			if (m_buffer_ptr->size() < L) {
				return std::nullopt;
			}
			const auto payload_size = get_payload_size(*m_buffer_ptr);
			if (m_buffer_ptr->size() < L + payload_size) {
				return std::nullopt;
			}
			const auto payload = read_payload(m_buffer_ptr, payload_size);
			return m_parser(payload);
		}
	private:
		RawDataBuffer *m_buffer_ptr;
		ApiRequestParser m_parser;
		static std::size_t get_payload_size(const RawDataBuffer& data) {
			std::size_t size(0UL);
			for (auto i = 0UL; i < L; ++i) {
				size <<= CHAR_BIT;
				size |= static_cast<std::size_t>(data.get(L - i - 1));
			}
			return size;
		}
		static std::vector<std::uint8_t> read_payload(RawDataBuffer *data, const std::size_t& payload_size) {
			std::vector<std::uint8_t> payload(payload_size, 0);
			for (auto i = 0; i < L; ++i) {
				data->pop_first();
			}
			for (auto i = 0; i < payload_size; ++i) {
				payload[i] = data->pop_first();
			}
			return payload;
		}
	};
}

#endif // API_REQUEST_READER_HPP