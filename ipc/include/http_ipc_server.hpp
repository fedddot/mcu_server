#ifndef	HTTP_IPC_SERVER_HPP
#define	HTTP_IPC_SERVER_HPP

#include <condition_variable>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <string>

#include "cpprest/http_msg.h"
#include "cpprest/http_listener.h"

#include "ipc_server.hpp"

namespace ipc {
	template <typename Trequest, typename Tresponse>
	class HttpIpcServer: public IpcServer<Trequest, Tresponse> {
	public:
		using HttpRequestToRequestTransformer = std::function<Trequest(const web::http::http_request&)>;
		using ResponseToHttpResponseTransformer = std::function<web::http::http_response(const Tresponse&)>;
		HttpIpcServer(
			const std::string& uri,
			const unsigned int polling_timeout_s,
			const unsigned int response_timeout_s,
			const HttpRequestToRequestTransformer& http_request_to_request,
			const ResponseToHttpResponseTransformer& response_to_http_response
		);
		HttpIpcServer(const HttpIpcServer&) = delete;
		HttpIpcServer& operator=(const HttpIpcServer&) = delete;
		
		~HttpIpcServer() noexcept override;

		void write(const Tresponse& response) const override;
		bool readable() const override;
		Trequest read() override;
	private:
		unsigned int m_polling_timeout_s;
		unsigned int m_response_timeout_s;
		HttpRequestToRequestTransformer m_http_request_to_request;
		ResponseToHttpResponseTransformer m_response_to_http_response;
		web::http::experimental::listener::http_listener m_listener;
		
		mutable std::unique_ptr<Tresponse> m_response;
		mutable std::unique_ptr<Trequest> m_request;

		mutable bool m_request_received;
		mutable std::mutex m_request_mux;
		mutable std::condition_variable m_request_cond;
		
		mutable bool m_response_received;
		mutable std::mutex m_response_mux;
		mutable std::condition_variable m_response_cond;
		
		void request_handler(const web::http::http_request& request);
	};

	template <typename Trequest, typename Tresponse>
	inline HttpIpcServer<Trequest, Tresponse>::HttpIpcServer(
		const std::string& uri,
		const unsigned int polling_timeout_s,
		const unsigned int response_timeout_s,
		const HttpRequestToRequestTransformer& http_request_to_request,
		const ResponseToHttpResponseTransformer& response_to_http_response
	):
		m_polling_timeout_s(polling_timeout_s),
		m_response_timeout_s(response_timeout_s),
		m_http_request_to_request(http_request_to_request),
		m_response_to_http_response(response_to_http_response),
		m_listener(uri),
		m_response(nullptr), m_request(nullptr) {

		m_listener.support(
			[this](web::http::http_request request) {
				request_handler(request);
			}
		);
		m_listener.open().wait();
	}

	template <typename Trequest, typename Tresponse>
	inline HttpIpcServer<Trequest, Tresponse>::~HttpIpcServer() noexcept {
		m_listener.close().wait();
	}

	template <typename Trequest, typename Tresponse>
	inline void HttpIpcServer<Trequest, Tresponse>::write(const Tresponse& outgoing_data) const {
		std::unique_lock response_lock(m_response_mux);
		m_response = std::make_unique<Tresponse>(outgoing_data);
		m_response_cond.notify_one();
	}

	template <typename Trequest, typename Tresponse>
	inline bool HttpIpcServer<Trequest, Tresponse>::readable() const {
		std::unique_lock lock(m_request_mux);
		if (m_request) {
			return true;
		}
		m_request_cond.wait_for(std::ref(lock), std::chrono::seconds(m_polling_timeout_s));
		return nullptr != m_request;
	}

	template <typename Trequest, typename Tresponse>
	Trequest HttpIpcServer<Trequest, Tresponse>::read() {
		std::unique_lock lock(m_request_mux);
		if (!m_request) {
			throw std::runtime_error("there is no request to read");
		}
		Trequest request(*m_request);
		m_request = nullptr;
		return request;
	}

	template <typename Trequest, typename Tresponse>
	inline void HttpIpcServer<Trequest, Tresponse>::request_handler(const web::http::http_request& request) {
		try {
			{
				std::unique_lock lock(m_request_mux);
				if (m_request) {
					request.reply(web::http::status_codes::TooManyRequests).wait();
					m_request_cond.notify_one();
					return;
				}
				m_request = std::make_unique<Trequest>(m_http_request_to_request(request));
				m_request_cond.notify_one();
			}
			std::unique_lock lock(m_response_mux);
			if (!m_response) {
				m_response_cond.wait_for(lock, std::chrono::seconds(m_response_timeout_s));
			}
			if (!m_response) {
				const auto timeout_response = web::http::http_response(web::http::status_codes::RequestTimeout);
				request.reply(timeout_response).wait();
				return;
			}
			request.reply(m_response_to_http_response(*m_response)).wait();
			m_response = nullptr;
		} catch (const std::exception& e) {
			auto response = web::http::http_response(web::http::status_codes::InternalError);
			response.set_body("\"" + std::string(e.what()) + "\"", utf8string("application/json; charset=utf-8"));
			request.reply(response).wait();
			m_request = nullptr;
			m_request_cond.notify_one();
			m_response = nullptr;
			m_response_cond.notify_one();
			return;
		}
	}
}

#endif // HTTP_IPC_SERVER_HPP