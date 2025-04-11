#ifndef	JSON_REQUEST_READER_HPP
#define	JSON_REQUEST_READER_HPP

#include <optional>
#include <stdexcept>

#include "request_reader.hpp"

namespace ipc {
	template <typename Request>
	class JsonRequestReader: public RequestReader<Request> {
	public:
		JsonRequestReader(

		);
		JsonRequestReader(const JsonRequestReader&) = delete;
		JsonRequestReader& operator=(const JsonRequestReader&) = delete;
		std::optional<Request> read() override;
	private:
	};

	template <typename Request>
	inline JsonRequestReader<Request>::JsonRequestReader(
	) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	template <typename Request>
	inline std::optional<Request> JsonRequestReader<Request>::read() {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // JSON_REQUEST_READER_HPP