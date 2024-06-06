#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

#include "creator.hpp"
#include "engine.hpp"

#include "gpio.hpp"

#include "data.hpp"
#include "gpo.hpp"
#include "inventory.hpp"
#include "object.hpp"
#include "integer.hpp"
#include "string.hpp"

#include "parser.hpp"
#include "serializer.hpp"
#include "server_creator.hpp"
#include "server_factory.hpp"
#include "server_inventory.hpp"
#include "server_parser.hpp"
#include "server_task.hpp"

namespace server {
	template <typename Traw_data, typename Tgpio_id>
	class Server {
	public:
		using RawDataParser = Parser<Data *(const Traw_data&)>;
		using RawDataSerializer = Serializer<Traw_data(const Data&)>;
		using GpioCreator = engine::Creator<Gpio *(const Tgpio_id&, const Gpio::Direction&)>;
		
		Server(const RawDataParser& parser, const RawDataSerializer& serializer, const GpioCreator& gpio_creator);
	
		Traw_data run(const Traw_data& data);
	private:
		std::unique_ptr<RawDataParser> m_parser;
		std::unique_ptr<RawDataSerializer> m_serializer;
		std::unique_ptr<GpioCreator> m_gpio_creator;

		std::unique_ptr<Inventory<Tgpio_id, Gpio>> m_gpio_inventory;

		using ServerEngine = engine::Engine<Data, Data *>;
		using ServerTask = engine::Engine<Data, Data *>::EngineTask;
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

	template <typename Traw_data, typename Tgpio_id>
	inline Server<Traw_data, Tgpio_id>::Server(const RawDataParser& parser, const RawDataSerializer& serializer, const GpioCreator& gpio_creator): m_parser(parser.clone()), m_serializer(serializer.clone()), m_gpio_creator(gpio_creator.clone()), m_gpio_inventory(new server_utl::ServerInventory<Tgpio_id, Gpio>()) {

		// init failure reporter
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

		// init task_id parser
		server_utl::ServerParser<ServerTaskId(const Data&)> task_id_parser(
			[](const Data& data)-> ServerTaskId {
				return Data::cast<String>(Data::cast<Object>(data).access("task_id")).get();
			}
		);
		
		// init factory
		server_utl::ServerFactory<Data, Data *, ServerTaskId> factory(task_id_parser);
		factory.register_creator(
			"create_gpio",
			server_utl::ServerCreator<ServerTask *(const Data&)>(
				[this](const Data& data)-> ServerTask * {
					return new server_utl::ServerTask<Data *>(
						[this, &data]()-> Data * {
							return create_gpio_task_action(data);
						}
					);
				}
			)
		);
		factory.register_creator(
			"set_gpio",
			server_utl::ServerCreator<ServerTask *(const Data&)>(
				[this](const Data& data)-> ServerTask * {
					return new server_utl::ServerTask<Data *>(
						[this, &data]()-> Data * {
							return set_gpio_task_action(data);
						}
					);
				}
			)
		);
		factory.register_creator(
			"delete_gpio",
			server_utl::ServerCreator<ServerTask *(const Data&)>(
				[this](const Data& data)-> ServerTask * {
					return new server_utl::ServerTask<Data *>(
						[this, &data]()-> Data * {
							return delete_gpio_task_action(data);
						}
					);
				}
			)
		);

		m_engine = std::make_unique<ServerEngine>(
			factory, 
			*m_failure_reporter
		);
	}

	template <typename Traw_data, typename Tgpio_id>
	inline Traw_data Server<Traw_data, Tgpio_id>::run(const Traw_data& data) {
		std::unique_ptr<Data> parsed_data(m_parser->parse(data));
		std::unique_ptr<Data> engine_report(m_engine->run_task(*parsed_data));
		return m_serializer->serialize(*engine_report);
	}

	template <typename Traw_data, typename Tgpio_id>
	inline Data *Server<Traw_data, Tgpio_id>::create_gpio_task_action(const Data& data) {
		auto gpio_id = Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get();
		auto gpio_dir = static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_dir")).get());
		m_gpio_inventory->put(gpio_id, m_gpio_creator->create(gpio_id, gpio_dir));
		Object report;
		report.add("result", Integer(0));
		return report.clone();
	}

	template <typename Traw_data, typename Tgpio_id>
	inline Data *Server<Traw_data, Tgpio_id>::set_gpio_task_action(const Data& data) {
		auto gpio_id = Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get();
		auto gpio_state = static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get());
		auto gpio_ptr = m_gpio_inventory->access(gpio_id);
		if (Gpio::Direction::OUT != gpio_ptr->direction()) {
			throw std::runtime_error("received GPIO is not an output");
		}
		Gpio::cast<Gpo>(*gpio_ptr).set_state(gpio_state);
		Object report;
		report.add("result", Integer(0));
		return report.clone();
	}

	template <typename Traw_data, typename Tgpio_id>
	inline Data *Server<Traw_data, Tgpio_id>::delete_gpio_task_action(const Data& data) {
		auto gpio_id = Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get();
		m_gpio_inventory->pull(gpio_id);
		Object report;
		report.add("result", Integer(0));
		return report.clone();
	}
}

#endif // SERVER_HPP