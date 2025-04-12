#ifndef	PB_REQUEST_READER_HPP
#define	PB_REQUEST_READER_HPP

#include <functional>
#include <optional>
#include <stdexcept>

#include "pb.h"

#include "pb_decode.h"
#include "request_reader.hpp"

namespace ipc {
	template <typename Request>
	class ProtobufRequestReader: public IpcDataReader<Request> {
	public:
		using InitialRequestCreator = std::function<Request()>;
		ProtobufRequestReader(
			pb_istream_t *input_stream_ptr,
			const pb_msgdesc_t *fields,
			const InitialRequestCreator& init_request_ctor
		);
		std::optional<Request> read() override;
	private:
		pb_istream_t *m_input_stream_ptr;
		const pb_msgdesc_t *m_fields;
		const InitialRequestCreator& m_init_request_ctor;
	};

	template <typename Request>
	inline ProtobufRequestReader<Request>::ProtobufRequestReader(
		pb_istream_t *input_stream_ptr,
		const pb_msgdesc_t *fields,
		const InitialRequestCreator& init_request_ctor
	): m_input_stream_ptr(input_stream_ptr), m_fields(fields), m_init_request_ctor(init_request_ctor) {
		if (!input_stream_ptr) {
			throw std::invalid_argument("invalid input stream ptr received");
		}
		if (!fields) {
			throw std::invalid_argument("invalid fields object ptr received");
		}
		if (!init_request_ctor) {
			throw std::invalid_argument("invalid init request ctor received");
		}
	}

	template <typename Request>
	inline std::optional<Request> ProtobufRequestReader<Request>::read() {
		auto request = m_init_request_ctor();
		if (!pb_decode_delimited(m_input_stream_ptr, m_fields, &request)) {
			return std::optional<Request>();
		}
		return std::optional<Request>(request);
	}
}

#endif // PB_REQUEST_READER_HPP