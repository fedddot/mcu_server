#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <memory>
#include <stdexcept>

#include "engine.hpp"
#include "connection.hpp"
#include "server_task_engine.hpp"

namespace server {
	template <typename Traw_data, typename Tgpio_id>
	class Server {
	public:
		using TaskIdParser = typename ServerTaskEngine<Traw_data, Tgpio_id>::TaskIdParser;
		using FailureReportCreator = typename ServerTaskEngine<Traw_data, Tgpio_id>::EngineFailureReportCreator;
		using RawDataParser = typename ServerTaskEngine<Traw_data, Tgpio_id>::EngineRawDataParser;
		using ReportSerializer = typename ServerTaskEngine<Traw_data, Tgpio_id>::EngineReportSerializer;
	
		using GpioIdParser = typename ServerTaskEngine<Traw_data, Tgpio_id>::GpioIdParser;
		using GpioDirParser = typename ServerTaskEngine<Traw_data, Tgpio_id>::GpioDirParser;
		using GpioStateParser = typename ServerTaskEngine<Traw_data, Tgpio_id>::GpioStateParser;
		
		using GpioCreator = typename ServerTaskEngine<Traw_data, Tgpio_id>::GpioCreator;


		Server(mcu_control::Connection<Traw_data> *connection, const TaskIdParser& task_id_parser, const FailureReportCreator& failure_report_creator, const RawDataParser& raw_data_parser, const ReportSerializer& report_serializer, const GpioIdParser& gpio_id_parser, const GpioDirParser& gpio_dir_parser, const GpioStateParser& gpio_state_parser, const GpioCreator& gpio_creator);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
	
		void run();
		bool is_running() const;
		void stop();
	private:
		mcu_control::Connection<Traw_data> *m_connection;
		std::unique_ptr<engine::Engine<Traw_data, Traw_data>> m_engine;
		bool m_is_running;
	};

	template <typename Traw_data, typename Tgpio_id>
	inline Server<Traw_data, Tgpio_id>::Server(mcu_control::Connection<Traw_data> *connection, const TaskIdParser& task_id_parser, const FailureReportCreator& failure_report_creator, const RawDataParser& raw_data_parser, const ReportSerializer& report_serializer, const GpioIdParser& gpio_id_parser, const GpioDirParser& gpio_dir_parser, const GpioStateParser& gpio_state_parser, const GpioCreator& gpio_creator): m_connection(connection), m_is_running(false) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
		m_engine = std::unique_ptr<engine::Engine<Traw_data, Traw_data>>(
			new ServerTaskEngine<Traw_data, Tgpio_id>(task_id_parser, failure_report_creator, raw_data_parser, report_serializer, gpio_id_parser, gpio_dir_parser, gpio_state_parser, gpio_creator)
		);
	}

	template <typename Traw_data, typename Tgpio_id>
	inline void Server<Traw_data, Tgpio_id>::run() {
		if (m_is_running) {
			throw std::runtime_error("server is already running");
		}
		m_is_running = true;
		while (m_is_running) {
			if (!m_connection->is_readable()) {
				continue;
			}
			Traw_data data = m_connection->read_data();
			m_connection->send_data(m_engine->run_task(data));
		}
	}

	template <typename Traw_data, typename Tgpio_id>
	inline bool Server<Traw_data, Tgpio_id>::is_running() const {
		return m_is_running;
	}

	template <typename Traw_data, typename Tgpio_id>
	inline void Server<Traw_data, Tgpio_id>::stop() {
		m_is_running = false;
	}
}

#endif // SERVER_HPP