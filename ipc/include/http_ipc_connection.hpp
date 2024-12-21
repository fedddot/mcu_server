#ifndef	HTTP_IPC_CONNECTION_HPP
#define	HTTP_IPC_CONNECTION_HPP

#include <condition_variable>
#include <mutex>
#include <string>

#include "cpprest/http_msg.h"
#include "cpprest/http_listener.h"

#include "data_reader.hpp"
#include "data_writer.hpp"
#include "request.hpp"
#include "response.hpp"

namespace ipc {
	
	class HttpIpcConnection: public DataReader<server::Request>, public DataWriter<server::Response> {
	public:
		HttpIpcConnection(const std::string& uri, const unsigned int polling_timeout_s, const unsigned int response_timeout_s);
		HttpIpcConnection(const HttpIpcConnection&) = delete;
		HttpIpcConnection& operator=(const HttpIpcConnection&) = delete;
		
		~HttpIpcConnection() noexcept override;

		void write(const server::Response& response) const override;
		bool readable() const override;
		server::Request read() const override;
	private:
		unsigned int m_polling_timeout_s;
		unsigned int m_response_timeout_s;
		web::http::experimental::listener::http_listener m_listener;
		
		class Promise {
		public:
			Promise(const unsigned int timeout_s);
			Promise(const Promise&) = delete;
			Promise& operator=(const Promise&) = delete;
			~Promise() noexcept = default;

			void set(const server::Response& response);
			server::Response get() const;
		private:
			unsigned int m_timeout_s;
			server::Response m_response;
			bool m_is_set;
			mutable std::mutex m_mux;
			mutable std::condition_variable m_cond;
		};

		Promise *m_promise;
		
		server::Request parse_request(const web::http::http_request& request);
		web::http::http_response parse_response(const server::Response& response);
		void request_handler(const web::http::http_request& request);
	};
}

#endif // HTTP_IPC_CONNECTION_HPP