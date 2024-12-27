#ifndef	SERVER_FIXTURE_HPP
#define	SERVER_FIXTURE_HPP

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "cpprest/http_client.h"
#include "cpprest/http_msg.h"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "pplx/pplxtasks.h"
#include "request.hpp"
#include "response.hpp"
#include "server_types.hpp"
#include "string.hpp"
#include "test_http_cnc_server.hpp"

namespace server_utl {
	class ServerFixture: public testing::Test {
	public:
		ServerFixture();
		ServerFixture(const ServerFixture& other) = delete;
		ServerFixture& operator=(const ServerFixture& other) = delete;

		void SetUp() override;
		void TearDown() override;

        server::Request create_gpio_request(const server::ResourceId& id, const manager::Gpio::Direction& dir, const int pin_num) const {
            using namespace server;
            Body request_body;
            request_body.add("id", String(id));
            Object cfg;
            cfg.add("direction", Integer(static_cast<int>(dir)));
            cfg.add("pin_number", Integer(pin_num));
            request_body.add("config", cfg);
            return Request(Request::Method::CREATE, {"gpios"}, request_body);
        }

        server::Response run_request(const server::Request& request) const {
            web::http::client::http_client client(m_test_server_url);
            auto request_task = client.request(to_http_request(request));
            if (pplx::task_status::completed != request_task.wait()) {
                throw std::runtime_error("failed to run http request");
            }
            return to_response(request_task.get());
        }
	private:
        const std::string m_test_server_url;
        const unsigned int m_poll_timeout_s;
        const unsigned int m_response_timeout_s;
		std::unique_ptr<cnc_server::TestHttpCncServer> m_test_server;
        std::thread m_server_thread;

        static web::http::http_request to_http_request(const server::Request& request);
        static server::Response to_response(const web::http::http_response& response);
	};

	inline ServerFixture::ServerFixture(): m_test_server_url("http://127.0.0.1:5555"), m_poll_timeout_s(1), m_response_timeout_s(10), m_test_server(nullptr) {
        
	}

	inline void ServerFixture::SetUp() {
        using namespace cnc_server;
		m_test_server = std::make_unique<TestHttpCncServer>(
            m_test_server_url,
            m_poll_timeout_s,
            m_response_timeout_s
        );
        m_server_thread = std::thread(
            [this](void) {
                m_test_server->run();
            }
        );
	}

    inline void ServerFixture::TearDown() {
        m_test_server->stop();
        m_server_thread.join();
    }

    inline web::http::http_request ServerFixture::to_http_request(const server::Request& request) {
        using namespace server;
        using namespace server_utl;
        const std::map<Request::Method, web::http::method> method_mapping {
            {Request::Method::CREATE, web::http::methods::POST},
            {Request::Method::READ, web::http::methods::GET},
            {Request::Method::UPDATE, web::http::methods::PUT},
            {Request::Method::DELETE, web::http::methods::DEL},
        };
        const auto method_iter = method_mapping.find(request.method());
        if (method_mapping.end() == method_iter) {
            throw std::invalid_argument("unsupported method received");
        }
        std::string uri("");
        for (const auto& path_element: request.path()) {
            uri.append("/");
            uri.append(path_element);
        }
        web::http::http_request http_request(method_iter->second);
        http_request.set_request_uri(uri);
        http_request.set_body(
            JsonDataSerializer()(request.body()),
            utf8string("application/json; charset=utf-8")
        );
        return http_request;
    }

    inline server::Response ServerFixture::to_response(const web::http::http_response& response) {
        using namespace server;
        using namespace server_utl;
        const std::map<web::http::status_code, ResponseCode> response_code_mapping {
            {web::http::status_codes::OK,               ResponseCode::OK},
            {web::http::status_codes::BadRequest,       ResponseCode::BAD_REQUEST},
            {web::http::status_codes::MethodNotAllowed, ResponseCode::METHOD_NOT_ALLOWED},
            {web::http::status_codes::NotFound,         ResponseCode::NOT_FOUND},
            {web::http::status_codes::RequestTimeout,   ResponseCode::TIMEOUT},
            {web::http::status_codes::InternalError,    ResponseCode::UNSPECIFIED}
        };
        const auto response_code_iter = response_code_mapping.find(response.status_code());
        if (response_code_mapping.end() == response_code_iter) {
            throw std::invalid_argument("unsupported response code");
        }
        
        const auto request_data_task = response.extract_string(true);
        if (pplx::task_status::completed != request_data_task.wait()) {
            throw std::runtime_error("failed to retrieve response data");
        }
        const auto request_data = request_data_task.get();
        const std::unique_ptr<Data> parsed_data(JsonDataParser()(request_data));
        const auto response_body = Data::cast<Body>(*parsed_data);
        return Response(response_code_iter->second, response_body);
    }
}

#endif // SERVER_FIXTURE_HPP