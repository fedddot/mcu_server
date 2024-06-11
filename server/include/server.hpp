#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <stdexcept>

#include "connection.hpp"
#include "engine.hpp"
#include "functional_listener.hpp"

namespace server {
	template <typename Traw_data, typename Tsubscriber_id>
	class Server {
	public:
		using Engine = engine::Engine<Traw_data, Traw_data>;
		using TaskFactory = typename Engine::EngineTaskFactory;
		using FailureReportCreator = typename Engine::EngineFailureReportCreator;
		using RawDataParser = typename Engine::EngineRawDataParser;
		using ReportSerializer = typename Engine::EngineReportSerializer;
		using ClientConnection = Connection<Tsubscriber_id, Traw_data>;
		
		Server(const TaskFactory& factory, const FailureReportCreator& failure_report_creator, const RawDataParser& raw_data_parser, const ReportSerializer& report_serializer, ClientConnection *connection);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
	
		void run();
		bool is_running() const;
		void stop();
	private:
		Engine m_engine;

		ClientConnection *m_connection;
		Tsubscriber_id m_subscriber_id;

		bool m_is_running;
	};

	template <typename Traw_data, typename Tsubscriber_id>
	inline Server<Traw_data, Tsubscriber_id>::Server(const TaskFactory& factory, const FailureReportCreator& failure_report_creator, const RawDataParser& raw_data_parser, const ReportSerializer& report_serializer, ClientConnection *connection): m_engine(factory, failure_report_creator, raw_data_parser, report_serializer), m_connection(connection), m_is_running(false) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	template <typename Traw_data, typename Tsubscriber_id>
	inline void Server<Traw_data, Tsubscriber_id>::run() {
		if (m_is_running) {
			throw std::runtime_error("server is already running");
		}
		m_subscriber_id = m_connection->subscribe(
			server_utl::FunctionalListener<const Traw_data&>(
				[this](const Traw_data& data) {
					auto report = m_engine.run_task(data);
					m_connection->send_data(report);
				}
			)
		);
		m_is_running = true;
	}

	template <typename Traw_data, typename Tsubscriber_id>
	inline bool Server<Traw_data, Tsubscriber_id>::is_running() const {
		return m_is_running;
	}

	template <typename Traw_data, typename Tsubscriber_id>
	inline void Server<Traw_data, Tsubscriber_id>::stop() {
		if (!m_is_running) {
			throw std::runtime_error("server is already not running");
		}
		m_connection->unsubscribe(m_subscriber_id);
		m_is_running = false;
	}
}

#endif // SERVER_HPP