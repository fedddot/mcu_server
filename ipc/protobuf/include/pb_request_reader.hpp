#ifndef	PB_REQUEST_READER_HPP
#define	PB_REQUEST_READER_HPP

#include <optional>

namespace ipc {
	template <typename Request>
	class ProtobufRequestReader {
	public:
		std::optional<Request> read() override;
	};
}

#endif // PB_REQUEST_READER_HPP