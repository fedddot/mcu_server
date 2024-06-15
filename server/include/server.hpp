#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <memory>
#include <stdexcept>

#include "connection.hpp"
#include "data.hpp"
#include "engine.hpp"
#include "functional_parser.hpp"
#include "gpio.hpp"
#include "parser.hpp"
#include "server_factory.hpp"
#include "server_tasks.hpp"

namespace server {
	template <typename Traw_data, typename Tgpio_id>
	class Server {
	public:
		using Connection = mcu_control::Connection<Traw_data>;
		
		using RawDataParser = engine::Parser<engine::Data *(const Traw_data&)>;
		using RawDataSerializer = engine::Serializer<Traw_data(const engine::Data&)>;

		Server(Connection *connection, const RawDataParser& raw_data_parser, const RawDataSerializer& raw_data_serializer);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
	
		void run();
		bool is_running() const;
		void stop();
	private:
		Connection *m_connection;
		
		std::unique_ptr<engine::Engine<Traw_data, Traw_data>> m_engine;
		bool m_is_running;
	};

	template <typename Traw_data, typename Tgpio_id>
	inline Server<Traw_data, Tgpio_id>::Server(Connection *connection, const RawDataParser& raw_data_parser, const RawDataSerializer& raw_data_serializer): m_connection(connection), m_engine(nullptr), m_is_running(false) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
		ServerFactory<Tgpio_id> factory(
			engine_utl::FunctionalParser<typename Signature>
		)
	}

	template <typename Traw_data, typename Tgpio_id>
	inline void Server<Traw_data, Tgpio_id>::run() {
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

	template <typename Traw_data, typename Tgpio_id>
	inline bool Server<Traw_data, Tgpio_id>::is_running() const {
		return m_is_running;
	}

	template <typename Traw_data, typename Tgpio_id>
	inline void Server<Traw_data, Tgpio_id>::stop() {
		if (!m_is_running) {
			throw std::runtime_error("server is already not running");
		}
		m_is_running = false;
	}
}

#endif // SERVER_HPP