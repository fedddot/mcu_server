#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <stdexcept>

#include "connection.hpp"
#include "engine.hpp"

namespace server {
	template <typename Traw_data, typename Ttime>
	class Server {
	public:
		using Engine = engine::Engine<Traw_data, Traw_data>;
		using TaskFactory = typename Engine::EngineTaskFactory;
		using FailureReportCreator = typename Engine::EngineFailureReportCreator;
		using RawDataParser = typename Engine::EngineRawDataParser;
		using ReportSerializer = typename Engine::EngineReportSerializer;
		using ClientConnection = mcu_control::Connection<Traw_data, Ttime>;
		
		Server(ClientConnection *connection, const Ttime& polling_period, const TaskFactory& factory, const FailureReportCreator& failure_report_creator, const RawDataParser& raw_data_parser, const ReportSerializer& report_serializer);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
	
		void run();
		bool is_running() const;
		void stop();
	private:
		Engine m_engine;

		ClientConnection *m_connection;
		Ttime m_polling_period;

		bool m_is_running;
	};

	template <typename Traw_data, typename Ttime>
	inline Server<Traw_data, Ttime>::Server(ClientConnection *connection, const Ttime& polling_period, const TaskFactory& factory, const FailureReportCreator& failure_report_creator, const RawDataParser& raw_data_parser, const ReportSerializer& report_serializer): m_connection(connection), m_polling_period(polling_period), m_engine(factory, failure_report_creator, raw_data_parser, report_serializer), m_is_running(false) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	template <typename Traw_data, typename Ttime>
	inline void Server<Traw_data, Ttime>::run() {
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

	template <typename Traw_data, typename Ttime>
	inline bool Server<Traw_data, Ttime>::is_running() const {
		return m_is_running;
	}

	template <typename Traw_data, typename Ttime>
	inline void Server<Traw_data, Ttime>::stop() {
		if (!m_is_running) {
			throw std::runtime_error("server is already not running");
		}
		m_is_running = false;
	}
}

#endif // SERVER_HPP