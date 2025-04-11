#ifndef	REQUEST_READER_HPP
#define	REQUEST_READER_HPP

#include <memory>
#include <optional>
#include <stdexcept>
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

	template <typename Request>
	class SharedRequestReader: public RequestReader<Request> {
	public:
		SharedRequestReader(RequestReader<Request> *instance_ptr);
		SharedRequestReader(const SharedRequestReader&) = default;
		SharedRequestReader& operator=(const SharedRequestReader&) = default;

		std::optional<Request> read() override;
	private:
		std::shared_ptr<RequestReader<Request>> m_instance_ptr;
	};

	template <typename Request>
	inline SharedRequestReader<Request>::SharedRequestReader(RequestReader<Request> *instance_ptr): m_instance_ptr(instance_ptr) {
		if (!m_instance_ptr) {
			throw std::invalid_argument("invalid instance ptr received");
		}
	}

	template <typename Request>
	inline std::optional<Request> SharedRequestReader<Request>::read() {
		return m_instance_ptr->read();
	}
}

#endif // REQUEST_READER_HPP