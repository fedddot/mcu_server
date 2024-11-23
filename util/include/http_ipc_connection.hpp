#ifndef	HTTP_IPC_CONNECTION_HPP
#define	HTTP_IPC_CONNECTION_HPP

#include <chrono>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

#include "cpprest/base_uri.h"
#include "cpprest/http_msg.h"
#include "cpprest/http_listener.h"

#include "data.hpp"
#include "ipc_connection.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_types.hpp"

namespace server_utl {
	template <typename Tsubscriber_id>
	class HttpIpcConnection: ipc::IpcConnection<Tsubscriber_id, server::Request, server::Response> {
	public:
		using Callback = typename ipc::IpcConnection<Tsubscriber_id, server::Request, server::Response>::Callback;
		
		HttpIpcConnection(const std::string& uri, const unsigned int response_timeout_s);
		HttpIpcConnection(const HttpIpcConnection&) = delete;
		HttpIpcConnection& operator=(const HttpIpcConnection&) = delete;
		
		~HttpIpcConnection() noexcept override;

		void subscribe(const Tsubscriber_id& id, const Callback& cb) override;
		void unsubscribe(const Tsubscriber_id& id) override;
		bool is_subscribed(const Tsubscriber_id& id) const override;
		void send(const server::Response& outgoing_data) const override;
	private:
		unsigned int m_response_timeout_s;
		web::http::experimental::listener::http_listener m_listener;
		std::map<Tsubscriber_id, Callback> m_subscribers;

		class Promise {
		public:
			Promise(const unsigned int timeout_s);
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

	template <typename Tsubscriber_id>
	inline HttpIpcConnection<Tsubscriber_id>::HttpIpcConnection(const std::string& uri, const unsigned int response_timeout_s): m_response_timeout_s(response_timeout_s), m_listener(uri), m_promise(nullptr) {
		m_listener.support(
			[this](web::http::http_request request) {
				request_handler(request);
			}
		);
		m_listener.open().wait();
	}
	
	template <typename Tsubscriber_id>
	inline HttpIpcConnection<Tsubscriber_id>::~HttpIpcConnection() noexcept {
		m_listener.close().wait();
	}
	
	template <typename Tsubscriber_id>
	inline void HttpIpcConnection<Tsubscriber_id>::subscribe(const Tsubscriber_id& id, const Callback& cb) {
		if (is_subscribed(id)) {
			throw std::invalid_argument("callback with received id is already subscribed");
		}
		m_subscribers.insert({id, cb});
	}
	
	template <typename Tsubscriber_id>
	inline void HttpIpcConnection<Tsubscriber_id>::unsubscribe(const Tsubscriber_id& id) {
		if (!is_subscribed(id)) {
			throw std::invalid_argument("callback with received id is not subscribed");
		}
		m_subscribers.erase(m_subscribers.find(id)); 
	}

	template <typename Tsubscriber_id>
	inline bool HttpIpcConnection<Tsubscriber_id>::is_subscribed(const Tsubscriber_id& id) const {
		return m_subscribers.end() != m_subscribers.find(id);
	}
	
	template <typename Tsubscriber_id>
	inline void HttpIpcConnection<Tsubscriber_id>::send(const server::Response& outgoing_data) const {
		if (!m_promise) {
			throw std::runtime_error("no one is waiting for response");
		}
		m_promise->set(outgoing_data);
	}

	template <typename Tsubscriber_id>
	inline server::Request HttpIpcConnection<Tsubscriber_id>::parse_request(const web::http::http_request& request) {
		using namespace server;
		auto retrieve_method = [](const web::http::http_request& request) {
			const std::map<std::string, Request::Method> mapping {
				{"POST",	Request::Method::CREATE},
				{"PUT",		Request::Method::UPDATE},
				{"DELETE",	Request::Method::DELETE},
				{"GET",		Request::Method::READ},
			};
			const auto iter = mapping.find(request.method());
			if (mapping.end() == iter) {
				throw std::invalid_argument("unsupported method");
			}
			return iter->second;
		};
		auto retrieve_route = [](const web::http::http_request& request) {
			const auto path_vect = web::uri::split_path(request.request_uri().to_string());
			return Path(path_vect.begin(), path_vect.end());
		};
		auto retrieve_body = [](const web::http::http_request& request) {
			auto data_task = request.extract_vector();
			data_task.wait();
			const auto data = data_task.get();
			const auto data_str = std::string(data.begin(), data.end());
			if (data_str.empty()) {
				return Body();
			}
			const std::unique_ptr<Data> parsed_data(JsonDataParser()(data_str));
			return Body(Data::cast<Body>(*parsed_data));
		};
		return Request(
			retrieve_method(request),
			retrieve_route(request),
			retrieve_body(request)
		);
	}
	
	template <typename Tsubscriber_id>
	inline web::http::http_response HttpIpcConnection<Tsubscriber_id>::parse_response(const server::Response& response) {
		using namespace server;
		auto retrieve_status_code = [](const Response& response) {
			switch (response.code()) {
			case ResponseCode::OK:
				return web::http::status_codes::OK;
			case ResponseCode::BAD_REQUEST:
				return web::http::status_codes::BadRequest;
			case ResponseCode::METHOD_NOT_ALLOWED:
				return web::http::status_codes::MethodNotAllowed;
			case ResponseCode::NOT_FOUND:
				return web::http::status_codes::NotFound;
			case ResponseCode::TIMEOUT:
				return web::http::status_codes::RequestTimeout;
			case ResponseCode::UNSPECIFIED:
				return web::http::status_codes::InternalError;
			default:
				throw std::invalid_argument("unsupported code");
			}
		};
		web::http::http_response http_resp(retrieve_status_code(response));
		http_resp.set_body(JsonDataSerializer()(response.body()));
		return http_resp;
	}

	template <typename Tsubscriber_id>
	inline void HttpIpcConnection<Tsubscriber_id>::request_handler(const web::http::http_request& request) {
		if (m_promise) {
			request.reply(web::http::status_codes::TooManyRequests).wait();
			return;
		}
		const auto parsed_request = parse_request(request);
		Promise promise(m_response_timeout_s);
		m_promise = &promise;
		for (const auto& [id, cb]: m_subscribers) {
			cb(parsed_request);
		}
		const auto response = promise.get();
		m_promise = nullptr;
		request.reply(parse_response(response)).wait();
	}

	template <typename Tsubscriber_id>
	inline HttpIpcConnection<Tsubscriber_id>::Promise::Promise(const unsigned int timeout_s): m_timeout_s(timeout_s), m_response(server::ResponseCode::TIMEOUT, server::Body()), m_is_set(false) {

	}

	template <typename Tsubscriber_id>
	inline void HttpIpcConnection<Tsubscriber_id>::Promise::set(const server::Response& response) {
		std::unique_lock lock(m_mux);
		if (m_is_set) {
			return;
		}
		m_response = response;
		m_is_set = true;
		m_cond.notify_all();
	}

	template <typename Tsubscriber_id>
	inline server::Response HttpIpcConnection<Tsubscriber_id>::Promise::get() const {
		std::unique_lock lock(m_mux);
		if (m_is_set) {
			return m_response;
		}
		m_cond.wait_for(lock, std::chrono::seconds(m_timeout_s));
		return m_response;
	}
}

#endif // HTTP_IPC_CONNECTION_HPP