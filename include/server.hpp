#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <exception>
#include <memory>
#include <string>

#include "engine.hpp"

#include "data.hpp"
#include "object.hpp"
#include "integer.hpp"
#include "string.hpp"

#include "parser.hpp"
#include "serializer.hpp"
#include "server_creator.hpp"

namespace server {
	template <typename Tinput_data, typename Treport_data>
	class Server {
	public:
		using ServerEngine = engine::Engine<Data, Data *>;
		using ServerTask = typename ServerEngine::EngineTask;
		using ServerTaskFactory = typename ServerEngine::EngineTaskFactory;

		using RawDataParser = Parser<Data *(const Tinput_data&)>;
		using RawDataSerializer = Serializer<Treport_data(const Data&)>;
		
		Server(const RawDataParser& parser, const RawDataSerializer& serializer, const ServerTaskFactory& factory);
	
		Tinput_data run(const Tinput_data& data);
	private:
		std::unique_ptr<RawDataParser> m_parser;
		std::unique_ptr<RawDataSerializer> m_serializer;

		using ServerFailureReportCreator = typename ServerEngine::EngineFailureReportCreator;
		using ServerTaskId = std::string;
		
		std::unique_ptr<ServerFailureReportCreator> m_failure_reporter;
		
		// tasks engine		
		std::unique_ptr<ServerEngine> m_engine;

		// tasks actions
		Data *create_gpio_task_action(const Data& data);
		Data *delete_gpio_task_action(const Data& data);
		Data *set_gpio_task_action(const Data& data);
	};

	template <typename Tinput_data, typename Tgpio_id>
	inline Server<Tinput_data, Tgpio_id>::Server(const RawDataParser& parser, const RawDataSerializer& serializer, const ServerTaskFactory& factory): m_parser(parser.clone()), m_serializer(serializer.clone()) {

		m_failure_reporter = std::unique_ptr<ServerFailureReportCreator>(
			new server_utl::ServerCreator<Data *(const std::exception& e)>(
				[](const std::exception& e)-> Data * {
					Object report;
					report.add("result", Integer(-1));
					report.add("what", String(std::string(e.what())));
					return report.clone();
				}
			)
		);

		m_engine = std::make_unique<ServerEngine>(
			factory, 
			*m_failure_reporter
		);
	}

	template <typename Tinput_data, typename Tgpio_id>
	inline Tinput_data Server<Tinput_data, Tgpio_id>::run(const Tinput_data& data) {
		std::unique_ptr<Data> parsed_data(m_parser->parse(data));
		std::unique_ptr<Data> engine_report(m_engine->run_task(*parsed_data));
		return m_serializer->serialize(*engine_report);
	}
}

#endif // SERVER_HPP