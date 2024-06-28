#ifndef	SERVER_TASK_ENGINE_HPP
#define	SERVER_TASK_ENGINE_HPP

#include <memory>
#include <string>

#include "create_gpio_task.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "delete_gpio_task.hpp"
#include "engine.hpp"
#include "functional_creator.hpp"
#include "get_gpio_task.hpp"
#include "gpio.hpp"
#include "inventory.hpp"
#include "parser.hpp"
#include "set_gpio_task.hpp"
#include "task_engine.hpp"
#include "factory.hpp"

namespace server {
	using ServerTaskId = std::string;

	template <typename Traw_data, typename Tgpio_id>
	class ServerTaskEngine: public engine::Engine<Traw_data, Traw_data> {
	public:
		using TaskIdParser = engine::Parser<ServerTaskId(const engine::Data&)>;
		using EngineFailureReportCreator = typename engine::TaskEngine<Traw_data, Traw_data>::EngineFailureReportCreator;
		using EngineRawDataParser = typename engine::TaskEngine<Traw_data, Traw_data>::EngineRawDataParser;
		using EngineReportSerializer = typename engine::TaskEngine<Traw_data, Traw_data>::EngineReportSerializer;
	
		using GpioIdParser = engine::Parser<Tgpio_id(const engine::Data&)>;
		using GpioDirParser = engine::Parser<Gpio::Direction(const engine::Data&)>;
		using GpioStateParser = engine::Parser<Gpio::State(const engine::Data&)>;
		
		using GpioCreator = engine::Creator<Gpio *(const Tgpio_id&, const Gpio::Direction&)>;

		
		ServerTaskEngine(const TaskIdParser& task_id_parser, const EngineFailureReportCreator& failure_report_creator, const EngineRawDataParser& raw_data_parser, const EngineReportSerializer& report_serializer, const GpioIdParser& gpio_id_parser, const GpioDirParser& gpio_dir_parser, const GpioStateParser& gpio_state_parser, const GpioCreator& gpio_creator);
		ServerTaskEngine(const ServerTaskEngine& other) = delete;
		ServerTaskEngine& operator=(const ServerTaskEngine& other) = delete;

		Traw_data run_task(const Traw_data& cfg) const override;
	private:
		std::unique_ptr<GpioIdParser> m_gpio_id_parser;
		std::unique_ptr<GpioDirParser> m_gpio_dir_parser;
		std::unique_ptr<GpioStateParser> m_gpio_state_parser;
		std::unique_ptr<GpioCreator> m_gpio_creator;

		using EngineTask = typename engine::TaskEngine<Traw_data, Traw_data>::EngineTask;
		std::unique_ptr<engine::Engine<Traw_data, Traw_data>> m_engine;
		Inventory<Tgpio_id, Gpio> m_gpio_inventory;
	};

	template <typename Traw_data, typename Tgpio_id>
	ServerTaskEngine<Traw_data, Tgpio_id>::ServerTaskEngine(const TaskIdParser& task_id_parser, const EngineFailureReportCreator& failure_report_creator, const EngineRawDataParser& raw_data_parser, const EngineReportSerializer& report_serializer, const GpioIdParser& gpio_id_parser, const GpioDirParser& gpio_dir_parser, const GpioStateParser& gpio_state_parser, const GpioCreator& gpio_creator): m_gpio_id_parser(gpio_id_parser.clone()), m_gpio_dir_parser(gpio_dir_parser.clone()), m_gpio_state_parser(gpio_state_parser.clone()), m_gpio_creator(gpio_creator.clone()) {
		Factory<ServerTaskId, EngineTask *, engine::Data> factory(task_id_parser);
		factory.register_creator(
			"create_gpio",
			engine_utl::FunctionalCreator<EngineTask *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> EngineTask * {
					return new CreateGpioTask<Tgpio_id>(
						&m_gpio_inventory,
						cfg,
						*m_gpio_id_parser,
						*m_gpio_dir_parser,
						*m_gpio_creator
					);
				}
			)
		);
		factory.register_creator(
			"set_gpio",
			engine_utl::FunctionalCreator<EngineTask *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> EngineTask * {
					return new SetGpioTask<Tgpio_id>(
						&m_gpio_inventory,
						cfg,
						*m_gpio_id_parser,
						*m_gpio_state_parser
					);
				}
			)
		);
		factory.register_creator(
			"get_gpio",
			engine_utl::FunctionalCreator<EngineTask *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> EngineTask * {
					return new GetGpioTask<Tgpio_id>(
						&m_gpio_inventory,
						cfg,
						*m_gpio_id_parser
					);
				}
			)
		);
		factory.register_creator(
			"delete_gpio",
			engine_utl::FunctionalCreator<EngineTask *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> EngineTask * {
					return new DeleteGpioTask<Tgpio_id>(
						&m_gpio_inventory,
						cfg,
						*m_gpio_id_parser
					);
				}
			)
		);
		m_engine = std::unique_ptr<engine::Engine<Traw_data, Traw_data>>(
			new engine::TaskEngine<Traw_data, Traw_data>(
				factory,
				failure_report_creator,
				raw_data_parser,
				report_serializer
			)
		);
	}

	template <typename Traw_data, typename Tgpio_id>
	Traw_data ServerTaskEngine<Traw_data, Tgpio_id>::run_task(const Traw_data& cfg) const {
		return m_engine->run_task(cfg);
	}
}

#endif // SERVER_TASK_ENGINE_HPP