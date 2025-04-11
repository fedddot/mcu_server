#ifndef	RESPONSE_WRITER_HPP
#define	RESPONSE_WRITER_HPP

#include <memory>
#include <stdexcept>

namespace ipc {
	/// @brief Synchronous IPC Responses Writer
	template <typename Response>
	class ResponseWriter {
	public:
		virtual ~ResponseWriter() noexcept = default;

		/// @brief Writes an IPC response into an IPC stream 
		virtual void write(const Response& response) const = 0;
	};

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

#endif // RESPONSE_WRITER_HPP