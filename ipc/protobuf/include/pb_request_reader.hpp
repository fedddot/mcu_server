#ifndef	PB_REQUEST_READER_HPP
#define	PB_REQUEST_READER_HPP

#include <optional>

#include "request_reader.hpp"

namespace ipc {
	template <typename Request>
	class ProtobufRequestReader: public RequestReader<Request> {
	public:
		ProtobufRequestReader() = default;
		std::optional<Request> read() override;
	};
}

#endif // PB_REQUEST_READER_HPP