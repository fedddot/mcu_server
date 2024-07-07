#ifndef	MCU_SERVER_HPP
#define	MCU_SERVER_HPP

#include <memory>
#include <stdexcept>

#include "data.hpp"
#include "data_receiver.hpp"
#include "data_sender.hpp"
#include "engine.hpp"
#include "listener.hpp"
#include "parser.hpp"
#include "serializer.hpp"

namespace mcu_server {
	template <typename Tgpio_id, typename Traw_data, typename Tfood>
	class McuServer {
	public:
		using McuEngine = engine::Engine<engine::Data *(const engine::Data&)>;
		using RawDataSender = DataSender<Traw_data>;
		using RawDataReceiver = DataReceiver<Traw_data, Tfood>;
		using RawDataParser = Parser<engine::Data *(const Traw_data&)>;
		using RawDataSerializer = Serializer<Traw_data(const engine::Data&)>;		
		
		McuServer(McuEngine *engine, RawDataSender *sender, RawDataReceiver *receiver, const RawDataParser& parser, const RawDataSerializer& serializer);
		McuServer(const McuServer& other) = delete;
		McuServer& operator=(const McuServer& other) = delete;
		virtual ~McuServer() noexcept = default;

		void feed(const Tfood& data);
	private:
		McuEngine *m_engine;
		RawDataSender *m_sender;
		RawDataReceiver *m_receiver;
		std::unique_ptr<RawDataParser> m_parser;
		std::unique_ptr<RawDataSerializer> m_serializer;

		class ServerListener: public Listener<const Traw_data&> {
		public:
			ServerListener(McuServer *server): m_server(server) {
				if (!m_server) {
					throw std::invalid_argument("invalid server ptr received");
				}
			}
			ServerListener(const ServerListener&) = default;
			ServerListener& operator=(const ServerListener&) = default;
			
			void on_event(const Traw_data& data) override {
				const std::unique_ptr<engine::Data> parsed_data(m_server->m_parser->parse(data));
				const std::unique_ptr<engine::Data> report(m_server->m_engine->run(*parsed_data));
				auto serialized_report = m_server->m_serializer->serialize(*report);
				m_server->m_sender->send(serialized_report);
			}
			Listener<const Traw_data&> *clone() const override {
				return new ServerListener(*this);
			}
		private:
			McuServer *m_server;
		};
	};

	template <typename Tgpio_id, typename Traw_data, typename Tfood>
	inline McuServer<Tgpio_id, Traw_data, Tfood>::McuServer(McuEngine *engine, RawDataSender *sender, RawDataReceiver *receiver, const RawDataParser& parser, const RawDataSerializer& serializer): m_engine(engine), m_sender(sender), m_receiver(receiver), m_parser(parser.clone()), m_serializer(serializer.clone()) {
		if (!engine || !sender || !receiver) {
			throw std::invalid_argument("invalid ptr received");
		}
		m_receiver->set_listener(ServerListener(this));
	}

	template <typename Tgpio_id, typename Traw_data, typename Tfood>
	void McuServer<Tgpio_id, Traw_data, Tfood>::feed(const Tfood& data) {
		m_receiver->feed(data);
	}
}

#endif // MCU_SERVER_HPP