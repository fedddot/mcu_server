#ifndef	CUSTOM_SERVER_HPP
#define	CUSTOM_SERVER_HPP

#include <exception>
#include <stdexcept>

#include "data_reader.hpp"
#include "data_writer.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "string.hpp"
#include "vendor.hpp"

namespace server_utl {
	class CustomServer: public server::Server {
	public:
		using RequestReader = ipc::DataReader<server::Request>;
		using ResponseWriter = ipc::DataWriter<server::Response>;
		CustomServer(
			RequestReader *request_reader,
			ResponseWriter *response_writer,
			server::Vendor *vendor
		);
		CustomServer(const CustomServer& other) = delete;
		CustomServer& operator=(const CustomServer& other) = delete;

		void run() override;
		bool is_running() const override;
		void stop() override;
	private:
		RequestReader *m_request_reader;
		ResponseWriter *m_response_writer;
		server::Vendor *m_vendor;
		bool m_is_running;
	};
	
	inline CustomServer::CustomServer(
		RequestReader *request_reader,
		ResponseWriter *response_writer,
		server::Vendor *vendor
	): m_request_reader(request_reader), m_response_writer(response_writer), m_vendor(vendor), m_is_running(false) {
		if (!m_request_reader || !m_response_writer || !m_vendor) {
			throw std::invalid_argument("invalid arguments received");
		}
	}

	
	inline void CustomServer::run() {
		using namespace server;
		m_is_running = true;
		while (m_is_running) {
			try {
				if (!m_request_reader->readable()) {
					continue;
				}
				const auto request = m_request_reader->read();
				const auto response = m_vendor->run_request(request);
				m_response_writer->write(response);
			} catch (const ServerException& e) {
				const auto failure_response = Response(e.code(), e.body());
				m_response_writer->write(failure_response);
				continue;
			} catch (const std::exception& e) {
				auto failure_response_body = Body();
				failure_response_body.add("what", String(e.what()));
				const auto failure_response = Response(ResponseCode::UNSPECIFIED, failure_response_body);
				m_response_writer->write(failure_response);
				continue;
			}
		}
	}

	inline bool CustomServer::is_running() const {
		return m_is_running;
	}

	inline void CustomServer::stop() {
		m_is_running = false;
	}
}

#endif // CUSTOM_SERVER_HPP