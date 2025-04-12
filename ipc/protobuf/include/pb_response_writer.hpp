#ifndef	PB_RESPONSE_WRITER_HPP
#define	PB_RESPONSE_WRITER_HPP

#include "ipc_data_writer.hpp"

namespace ipc {
	template <typename Response>
	class ProtobufResponseWriter: public ResponseWriter<Response> {
	public:
		void write(const Response& response) const override;
	};
}

#endif // PB_RESPONSE_WRITER_HPP