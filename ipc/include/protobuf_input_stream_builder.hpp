#ifndef	PROTOBUF_INPUT_STREAM_BUILDER_HPP
#define	PROTOBUF_INPUT_STREAM_BUILDER_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "pb.h"
#include "pb_decode.h"

namespace ipc {
	class ProtobufInputStreamBuilder {
	public:
		ProtobufInputStreamBuilder(const std::vector<pb_byte_t> *buffer);
		ProtobufInputStreamBuilder(const ProtobufInputStreamBuilder&) = delete;
		ProtobufInputStreamBuilder& operator=(const ProtobufInputStreamBuilder&) = delete;
		virtual ~ProtobufInputStreamBuilder() noexcept = default;
		
		pb_istream_t build() const;
	private:
		const std::vector<pb_byte_t> *m_buffer;
		std::size_t m_pos;
		static bool read_from_pb_buffer(pb_istream_t *stream, pb_byte_t *buf, size_t count);
	};

	inline ProtobufInputStreamBuilder::ProtobufInputStreamBuilder(const std::vector<pb_byte_t> *buffer): m_buffer(buffer), m_pos(0UL) {
		if (!m_buffer) {
			throw std::invalid_argument("invalid buffer ptr received");
		}
	}
	
	inline pb_istream_t ProtobufInputStreamBuilder::build() const {
		return pb_istream_s {
			.callback = read_from_pb_buffer,
			.state = const_cast<ProtobufInputStreamBuilder *>(this),
			.bytes_left = m_buffer->size() - m_pos,
			.errmsg = nullptr,
		};
	}
		
	inline bool ProtobufInputStreamBuilder::read_from_pb_buffer(pb_istream_t *stream, pb_byte_t *buf, size_t count) {
		if (!stream) {
			return false;
		}
		if (!buf) {
			static const auto invalid_args_msg = "buffer reader received invalid buffer ptr";
			stream->errmsg = invalid_args_msg;
			return false;
		}
		auto stream_ptr = static_cast<ProtobufInputStreamBuilder *>(stream->state);
		const auto start_position = stream_ptr->m_pos;
		const auto end_position = start_position + count;
		if (stream_ptr->m_buffer->size() < end_position) {
			static const auto invalid_format_msg = "attempt to read more bytes than the buffer has";
			stream->errmsg = invalid_format_msg;
			return false;
		}
		for (auto i = start_position; i < end_position; ++i) {
			buf[i] = stream_ptr->m_buffer->at(i);
		}
		stream_ptr->m_pos = end_position;
		return true;
	}
}

#endif // PROTOBUF_INPUT_STREAM_BUILDER_HPP