#ifndef	SHARED_RESPONSE_WRITER_HPP
#define	SHARED_RESPONSE_WRITER_HPP

#include <memory>
#include <stdexcept>

#include "response_writer.hpp"

namespace ipc {
	template <typename Response>
	class SharedResponseWriter: public ResponseWriter<Response> {
	public:
		SharedResponseWriter(ResponseWriter<Response> *instance_ptr);
		SharedResponseWriter(const SharedResponseWriter&) = default;
		SharedResponseWriter& operator=(const SharedResponseWriter&) = default;

		void write(const Response& response) const override;
	private:
		std::shared_ptr<ResponseWriter<Response>> m_instance_ptr;
	};

	template <typename Response>
	inline SharedResponseWriter<Response>::SharedResponseWriter(ResponseWriter<Response> *instance_ptr): m_instance_ptr(instance_ptr) {
		if (!m_instance_ptr) {
			throw std::invalid_argument("invalid instance ptr received");
		}
	}

	template <typename Response>
	inline void SharedResponseWriter<Response>::write(const Response& response) const {
		m_instance_ptr->write(response);
	}
}

#endif // SHARED_RESPONSE_WRITER_HPP