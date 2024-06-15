#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <exception>
#include <memory>
#include <string>

#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "string.hpp"

#include "engine.hpp"
#include "functional_creator.hpp"

#include "parser.hpp"
#include "functional_parser.hpp"

#include "gpio.hpp"
#include "inventory.hpp"

#include "server_tasks.hpp"
#include "server_factory.hpp"

namespace server {
	template <typename Traw_data, typename Tgpio_id>
	class Server {
	public:
		using RawDataParser = engine::Parser<engine::Data *(const Traw_data&)>;
		using RawDataSerializer = engine::Serializer<Traw_data(const engine::Data&)>;

		using GpioCreator = engine::Creator<Gpio *(const Tgpio_id&, const Gpio::Direction&)>;
		using GpioIdParser = engine::Parser<Tgpio_id(const engine::Data&)>;

		Server(const RawDataParser& raw_data_parser, const RawDataSerializer& raw_data_serializer, const GpioCreator& gpio_creator, const GpioIdParser& gpio_id_parser);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
	
		Traw_data run(const Traw_data& data);
	private:
		Inventory<Tgpio_id, Gpio> m_gpio_inventory;
		
		std::unique_ptr<engine::Engine<Traw_data, Traw_data>> m_engine;
	};

	template <typename Traw_data, typename Tgpio_id>
	inline Server<Traw_data, Tgpio_id>::Server(const RawDataParser& raw_data_parser, const RawDataSerializer& raw_data_serializer, const GpioCreator& gpio_creator, const GpioIdParser& gpio_id_parser): m_engine(nullptr) {
		ServerFactory<Tgpio_id> factory(
			engine_utl::FunctionalParser<TaskId(const engine::Data&)>(
				[](const engine::Data& data)-> TaskId {
					return static_cast<TaskId>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("task_id")).get());
				}
			),
			&m_gpio_inventory,
			gpio_creator,
			gpio_id_parser,
			engine_utl::FunctionalParser<Gpio::Direction(const engine::Data&)>(
				[](const engine::Data& data)-> Gpio::Direction {
					return static_cast<Gpio::Direction>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("gpio_dir")).get());
				}
			),
			engine_utl::FunctionalParser<Gpio::State(const engine::Data&)>(
				[](const engine::Data& data)-> Gpio::State {
					return static_cast<Gpio::State>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("gpio_state")).get());
				}
			)
		);
		m_engine = std::unique_ptr<engine::Engine<Traw_data, Traw_data>>(
			new engine::Engine<Traw_data, Traw_data>(
				factory,
				engine_utl::FunctionalCreator<engine::Data *(const std::exception&)>(
					[](const std::exception& e) -> engine::Data * {
						engine::Object report;
						report.add("result", engine::Integer(-1));
						report.add("what", engine::String(std::string(e.what())));
						return report.clone();
					}
				),
				raw_data_parser,
				raw_data_serializer
			)
		);
	}

	template <typename Traw_data, typename Tgpio_id>
	inline Traw_data Server<Traw_data, Tgpio_id>::run(const Traw_data& data) {
		return m_engine->run_task(data);
	}
}

#endif // SERVER_HPP