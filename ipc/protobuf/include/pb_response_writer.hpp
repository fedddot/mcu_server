#ifndef	PB_RESPONSE_WRITER_HPP
#define	PB_RESPONSE_WRITER_HPP

namespace ipc {
	template <typename Response>
	class ProtobufResponseWriter {
	public:
		void write(const Response& response) const override;
	};
}

#endif // PB_RESPONSE_WRITER_HPP