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

#include "ipc_config.hpp"
#include "ipc_option.hpp"
#include "ipc_server.hpp"

namespace ipc {
	template <typename Request, typename Response>
	class HttpIpcServerConfig: public IpcConfig {
	public:
		using ToRequestTransformer = std::function<Request(const web::http::http_request&)>;
		using ToResponseTransformer = std::function<web::http::http_response(const Response&)>;
		
		HttpIpcServerConfig() = default;
		HttpIpcServerConfig(const HttpIpcServerConfig&) = default;
		HttpIpcServerConfig& operator=(const HttpIpcServerConfig&) = default;

		std::string type() const override;
		
		std::string uri;
		unsigned int polling_timeout_s;
		unsigned int response_timeout_s;
		ToRequestTransformer to_request;
		ToResponseTransformer to_response;
	};

	template <typename Request, typename Response>
	inline std::string HttpIpcServerConfig<Request, Response>::type() const {
		return std::string("ipc.server.http");
	}

	template <typename Request, typename Response>
	class HttpIpcServer: public IpcServer<Request, Response> {
	public:
		HttpIpcServer(const HttpIpcServerConfig<Request, Response>& config);
		HttpIpcServer(const HttpIpcServer&) = delete;
		HttpIpcServer& operator=(const HttpIpcServer&) = delete;
		
		~HttpIpcServer() noexcept override;

		void write(const Response& response) const override;
		Option<Request> read() override;
	private:
		HttpIpcServerConfig<Request, Response> m_config;
		web::http::experimental::listener::http_listener m_listener;
		
		mutable std::unique_ptr<Response> m_response;
		mutable std::unique_ptr<Request> m_request;

		mutable bool m_request_received;
		mutable std::mutex m_request_mux;
		mutable std::condition_variable m_request_cond;
		
		mutable bool m_response_received;
		mutable std::mutex m_response_mux;
		mutable std::condition_variable m_response_cond;
		
		void request_handler(const web::http::http_request& request);
	};

	template <typename Request, typename Response>
	inline HttpIpcServer<Request, Response>::HttpIpcServer(const HttpIpcServerConfig<Request, Response>& config):
		m_config(config),
		m_listener(config.uri),
		m_response(nullptr), m_request(nullptr) {

		m_listener.support(
			[this](web::http::http_request request) {
				request_handler(request);
			}
		);
		m_listener.open().wait();
	}

	template <typename Request, typename Response>
	inline HttpIpcServer<Request, Response>::~HttpIpcServer() noexcept {
		m_listener.close().wait();
	}

	template <typename Request, typename Response>
	inline void HttpIpcServer<Request, Response>::write(const Response& outgoing_data) const {
		std::unique_lock response_lock(m_response_mux);
		m_response = std::make_unique<Response>(outgoing_data);
		m_response_cond.notify_one();
	}

	template <typename Request, typename Response>
	Option<Request> HttpIpcServer<Request, Response>::read() {
		std::unique_lock lock(m_request_mux);
		if (!m_request) {
			m_request_cond.wait_for(std::ref(lock), std::chrono::seconds(m_config.polling_timeout_s));
		}
		if (!m_request) {
			return Option<Request>(nullptr);
		}
		const auto result = Option<Request>(new Request(*m_request));
		m_request = nullptr;
		return result;
	}

	template <typename Request, typename Response>
	inline void HttpIpcServer<Request, Response>::request_handler(const web::http::http_request& request) {
		try {
			{
				std::unique_lock lock(m_request_mux);
				if (m_request) {
					request.reply(web::http::status_codes::TooManyRequests).wait();
					m_request_cond.notify_one();
					return;
				}
				m_request = std::make_unique<Request>(m_config.to_request(request));
				m_request_cond.notify_one();
			}
			std::unique_lock lock(m_response_mux);
			if (!m_response) {
				m_response_cond.wait_for(lock, std::chrono::seconds(m_config.response_timeout_s));
			}
			if (!m_response) {
				const auto timeout_response = web::http::http_response(web::http::status_codes::RequestTimeout);
				request.reply(timeout_response).wait();
				return;
			}
			request.reply(m_config.to_response(*m_response)).wait();
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