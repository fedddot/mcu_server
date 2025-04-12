#ifndef	JSON_RESPONSE_WRITER_HPP
#define	JSON_RESPONSE_WRITER_HPP

#include <stdexcept>

#include "ipc_data_writer.hpp"

namespace ipc {
	template <typename Response>
	class JsonIpcDataWriter: public IpcDataWriter<Response> {
	public:
		JsonIpcDataWriter(
		);
		JsonIpcDataWriter(const JsonIpcDataWriter&) = delete;
		JsonIpcDataWriter& operator=(const JsonIpcDataWriter&) = delete;
		void write(const Response& response) const override;
	private:
	};

	template <typename Response>
	inline JsonIpcDataWriter<Response>::JsonIpcDataWriter(
	) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	template <typename Response>
	inline void JsonIpcDataWriter<Response>::write(const Response& response) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // JSON_RESPONSE_WRITER_HPP