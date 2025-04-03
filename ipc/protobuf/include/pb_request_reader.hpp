#ifndef	PB_REQUEST_READER_HPP
#define	PB_REQUEST_READER_HPP

#include <optional>
#include <stdexcept>

#include "pb.h"

#include "pb_decode.h"
#include "request_reader.hpp"

namespace ipc {
	template <typename Request>
	class ProtobufRequestReader: public RequestReader<Request> {
	public:
		ProtobufRequestReader(
			pb_istream_t *input_stream_ptr,
			const pb_msgdesc_t *fields
		);
		std::optional<Request> read() override;
	private:
		pb_istream_t *m_input_stream_ptr;
		const pb_msgdesc_t *m_fields;
	};

	template <typename Request>
	inline ProtobufRequestReader<Request>::ProtobufRequestReader(
		pb_istream_t *input_stream_ptr,
		const pb_msgdesc_t *fields
	): m_input_stream_ptr(input_stream_ptr), m_fields(fields) {
		if (!input_stream_ptr) {
			throw std::invalid_argument("invalid input stream ptr received");
		}
		if (!fields) {
			throw std::invalid_argument("invalid fields object ptr received");
		}
	}

	template <typename Request>
	inline std::optional<Request> ProtobufRequestReader<Request>::read() {
		auto request = Request();
		if (!pb_decode(m_input_stream_ptr, m_fields, &request)) {
			return std::optional<Request>();
		}
		return std::optional<Request>(request);
	}
}

#endif // PB_REQUEST_READER_HPP