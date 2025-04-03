#ifndef	PB_REQUEST_READER_HPP
#define	PB_REQUEST_READER_HPP

#include <optional>
#include <stdexcept>

#include "request_reader.hpp"

namespace ipc {
	template <typename Request>
	class ProtobufRequestReader: public RequestReader<Request> {
	public:
		ProtobufRequestReader() = default;
		std::optional<Request> read() override;
	};

	template <typename Request>
	inline std::optional<Request> ProtobufRequestReader<Request>::read() {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // PB_REQUEST_READER_HPP