#ifndef	REQUEST_READER_HPP
#define	REQUEST_READER_HPP

#include <optional>

namespace ipc {
	/// @brief Synchronous IPC Requests Reader
	template <typename Request>
	class RequestReader {
	public:
		virtual ~RequestReader() noexcept = default;

		/// @brief Reads a single request (if exists) from an IPC stream and returns it as an optional value 
		/// @return if there is an incoming request returns non-empty option, an empty one otherwise
		virtual std::optional<Request> read() = 0;
	};
}

#endif // REQUEST_READER_HPP