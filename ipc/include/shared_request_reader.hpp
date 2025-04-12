#ifndef	SHARED_REQUEST_READER_HPP
#define	SHARED_REQUEST_READER_HPP

#include <memory>
#include <optional>
#include <stdexcept>

#include "request_reader.hpp"

namespace ipc {
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

#endif // SHARED_REQUEST_READER_HPP