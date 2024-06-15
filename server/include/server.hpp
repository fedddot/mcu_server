#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <stdexcept>

#include "connection.hpp"
#include "data.hpp"
#include "engine.hpp"
#include "parser.hpp"

namespace server {
	template <typename Traw_data>
	class Server {
	public:
		using RawDataParser = engine::Parser<engine::Data *(const Traw_data&)>;
		using RawDataSerializer = engine::Serializer<Traw_data(const engine::Data&)>;
		using Connection = mcu_control::Connection<Traw_data>;

		Server(Connection *connection);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
	
		void run();
		bool is_running() const;
		void stop();
	private:
		Connection *m_connection;
		
		engine::Engine<Traw_data, Traw_data> m_engine;
		bool m_is_running;
	};

	template <typename Traw_data>
	inline Server<Traw_data>::Server(ClientConnection *connection, const FailureReportCreator& failure_report_creator, const RawDataParser& raw_data_parser, const ReportSerializer& report_serializer): m_connection(connection), m_engine(factory, failure_report_creator, raw_data_parser, report_serializer), m_is_running(false) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	template <typename Traw_data>
	inline void Server<Traw_data>::run() {
		if (m_is_running) {
			throw std::runtime_error("server is already running");
		}
		m_is_running = true;
		while (m_is_running) {
			if (!m_connection->poll_data(m_polling_period)) {
				continue;
			}
			m_connection->send_data(m_engine.run_task(m_connection->read_data()));
		}
	}

	template <typename Traw_data>
	inline bool Server<Traw_data>::is_running() const {
		return m_is_running;
	}

	template <typename Traw_data>
	inline void Server<Traw_data>::stop() {
		if (!m_is_running) {
			throw std::runtime_error("server is already not running");
		}
		m_is_running = false;
	}
}

#endif // SERVER_HPP