#ifndef	JSON_RESPONSE_WRITER_HPP
#define	JSON_RESPONSE_WRITER_HPP

#include <stdexcept>

#include "ipc_data_writer.hpp"

namespace ipc {
	template <typename Response>
	class JsonResponseWriter: public IpcDataWriter<Response> {
	public:
		JsonResponseWriter(
		);
		JsonResponseWriter(const JsonResponseWriter&) = delete;
		JsonResponseWriter& operator=(const JsonResponseWriter&) = delete;
		void write(const Response& response) const override;
	private:
	};

	template <typename Response>
	inline JsonResponseWriter<Response>::JsonResponseWriter(
	) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	template <typename Response>
	inline void JsonResponseWriter<Response>::write(const Response& response) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // JSON_RESPONSE_WRITER_HPP