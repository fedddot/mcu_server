#ifndef	HTTP_IPC_SERVER_HPP
#define	HTTP_IPC_SERVER_HPP

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <mutex>
#include <string>

#include "cpprest/http_msg.h"
#include "cpprest/http_listener.h"

#include "ipc_server.hpp"
#include "ipc_request_handler.hpp"

namespace ipc {
	template <typename Request, typename Response>
	class HttpIpcServer: public IpcServer<Request, Response> {
	public:
		using HttpToRequestTransformer = std::function<Request(const web::http::http_request&)>;
		using ResponseToHttpTransformer = std::function<web::http::http_response(const Response&)>;

		HttpIpcServer(
			const std::string& uri,
			const HttpToRequestTransformer& http2request_transformer,
			const ResponseToHttpTransformer& response2http_transformer
		);
		HttpIpcServer(const HttpIpcServer&) = delete;
		HttpIpcServer& operator=(const HttpIpcServer&) = delete;

		void serve(IpcRequestHandler<Request, Response> *request_handler) override;
		void stop() override;
	private:
		std::string m_uri;
		const HttpToRequestTransformer& m_http2request_transformer;
		const ResponseToHttpTransformer& m_response2http_transformer;

		std::atomic_bool m_is_running;
	};

	template <typename Request, typename Response>
	inline HttpIpcServer<Request, Response>::HttpIpcServer(
		const std::string& uri,
		const HttpToRequestTransformer& http2request_transformer,
		const ResponseToHttpTransformer& response2http_transformer
	): m_uri(uri), m_http2request_transformer(http2request_transformer), m_response2http_transformer(response2http_transformer), m_is_running(false) {
	
	}

	template <typename Request, typename Response>
	inline void HttpIpcServer<Request, Response>::serve(IpcRequestHandler<Request, Response> *request_handler) {
		auto mux = std::mutex();
		auto cond = std::condition_variable();
		
		auto listener = web::http::experimental::listener::http_listener(m_uri);
		listener.support(
			[this, request_handler, &mux, &cond](web::http::http_request http_request) {
				auto lock = std::unique_lock(mux);
				try {
					const auto request = m_http2request_transformer(http_request);
					const auto response = request_handler->handle(request);
					const auto http_response = m_response2http_transformer(response);
					http_request.reply(http_response).wait();
				} catch (const std::exception& e) {
					auto error_response = web::http::http_response(web::http::status_codes::InternalError);
					const auto body = std::string("{\"what\": \"") + std::string(e.what()) + std::string("\"}");
					error_response.set_body(body);
					http_request.reply(error_response).wait();
				}
				cond.notify_all();
			}
		);
		listener.open().wait();
		m_is_running = true;
		while (m_is_running) {
			auto lock = std::unique_lock(mux);
			cond.wait_for(lock, std::chrono::milliseconds(500UL));
		}
		listener.close().wait();
	}

	template <typename Request, typename Response>
	inline void HttpIpcServer<Request, Response>::stop() {
		m_is_running = false;
	}
}

#endif // HTTP_IPC_SERVER_HPP