#ifndef	PB_REQUEST_READER_HPP
#define	PB_REQUEST_READER_HPP

#include "request_reader.hpp"
#include <optional>

namespace ipc {
	template <typename Request>
	class ProtobufRequestReader: public RequestReader<Request> {
	public:
		std::optional<Request> read() override;
	};
}

#endif // PB_REQUEST_READER_HPP