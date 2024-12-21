#include <stdexcept>

#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "request.hpp"
#include "response.hpp"

#include "http_ipc_connection.hpp"

using namespace ipc;
using namespace server;
using namespace server_utl;
	
HttpIpcConnection::HttpIpcConnection(const std::string& uri, const unsigned int polling_timeout_s, const unsigned int response_timeout_s):
	m_polling_timeout_s(polling_timeout_s),
	m_response_timeout_s(response_timeout_s),
	m_listener(uri),
	m_promise(nullptr) {
	m_listener.support(
		[this](web::http::http_request request) {
			request_handler(request);
		}
	);
	m_listener.open().wait();
}

HttpIpcConnection::~HttpIpcConnection() noexcept {
	m_listener.close().wait();
}

void HttpIpcConnection::write(const Response& outgoing_data) const {
	if (!m_promise) {
		throw std::runtime_error("no one is waiting for response");
	}
	m_promise->set(outgoing_data);
}

bool HttpIpcConnection::readable() const {
	throw std::runtime_error("NOT IMPLEMENTED");
}

Request HttpIpcConnection::read() const {
	throw std::runtime_error("NOT IMPLEMENTED");
}

Request HttpIpcConnection::parse_request(const web::http::http_request& request) {
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


web::http::http_response HttpIpcConnection::parse_response(const Response& response) {
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

void HttpIpcConnection::request_handler(const web::http::http_request& request) {
	if (m_promise) {
		request.reply(web::http::status_codes::TooManyRequests).wait();
		return;
	}
	const auto parsed_request = parse_request(request);
	Promise promise(m_response_timeout_s);
	m_promise = &promise;
	throw std::runtime_error("UNDERIMPLEMENTED");
	const auto response = promise.get();
	m_promise = nullptr;
	request.reply(parse_response(response)).wait();
}

HttpIpcConnection::Promise::Promise(const unsigned int timeout_s): m_timeout_s(timeout_s), m_response(ResponseCode::TIMEOUT, Body()), m_is_set(false) {

}

void HttpIpcConnection::Promise::set(const Response& response) {
	std::unique_lock lock(m_mux);
	if (m_is_set) {
		return;
	}
	m_response = response;
	m_is_set = true;
	m_cond.notify_all();
}

Response HttpIpcConnection::Promise::get() const {
	std::unique_lock lock(m_mux);
	if (m_is_set) {
		return m_response;
	}
	m_cond.wait_for(lock, std::chrono::seconds(m_timeout_s));
	return m_response;
}