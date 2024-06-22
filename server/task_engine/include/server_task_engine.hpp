#ifndef	SERVER_TASK_ENGINE_HPP
#define	SERVER_TASK_ENGINE_HPP

#include "creator.hpp"
#include "data.hpp"
#include "engine.hpp"
#include "functional_creator.hpp"
#include "functional_task.hpp"
#include "gpi.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "object.hpp"
#include "parser.hpp"
#include "server_tasks.hpp"
#include "task.hpp"
#include "task_factory.hpp"

#include <memory>
#include <stdexcept>

namespace server {
	using ServerRawData = std::string;

	template <typename Tgpio_id>
	class ServerTaskEngine: public engine::Engine<ServerRawData, ServerRawData> {
	public:
		using GpioIdParser = engine::Parser<Tgpio_id(const engine::Data&)>;
		using GpioDirParser = engine::Parser<Gpio::Direction(const engine::Data&)>;
		using GpioStateParser = engine::Parser<Gpio::State(const engine::Data&)>;
		
		using GpioCreator = engine::Creator<Gpio *(const Tgpio_id&, const Gpio::Direction&)>;
		
		ServerTaskEngine(const GpioIdParser& gpio_id_parser, const GpioDirParser& gpio_dir_parser, const GpioStateParser& gpio_state_parser, const GpioCreator& gpio_creator);
		ServerTaskEngine(const ServerTaskEngine& other) = delete;
		ServerTaskEngine& operator=(const ServerTaskEngine& other) = delete;
	private:
		std::unique_ptr<GpioIdParser> m_gpio_id_parser;
		std::unique_ptr<GpioDirParser> m_gpio_dir_parser;
		std::unique_ptr<GpioStateParser> m_gpio_state_parser;
		std::unique_ptr<GpioCreator> m_gpio_creator;
	};

	template <typename Tgpio_id>
	inline engine::Data *ServerTaskEngine<Tgpio_id>::create_gpio(const engine::Data& cfg) {
		return new InventoryTask<Tgpio_id, Gpio>(
			m_gpio_inventory,
			cfg,
			[&m_gpio_id_parser](Inventory<Tgpio_id, Gpio> *inventory_ptr, const engine::Data& cfg_data)-> engine::Data * {
				auto gpio_id = m_gpio_id_parser->parse(cfg);
				auto gpio_dir = m_gpio_dir_parser->parse(cfg);
				m_gpio_inventory->put(
					gpio_id,
					m_gpio_creator->create(gpio_id, gpio_dir)
				);
				engine::Object report;
				report.add("result", engine::Integer(0));
				return report.clone();
			}
		);
			[*this, gpio_id, gpio_dir]()-> engine::Data * {
				
			}
		);
	}

	template <typename Tgpio_id>
	inline engine::Task<engine::Data *> *ServerTaskEngine<Tgpio_id>::create_gpio_delete_task(const engine::Data& cfg) {
		auto gpio_id = m_gpio_id_parser->parse(cfg);
		return new engine_utl::FunctionalTask<engine::Data *>(
			[*this, gpio_id]()-> engine::Data * {
				delete m_gpio_inventory->pull(gpio_id);
				engine::Object report;
				report.add("result", engine::Integer(0));
				return report.clone();
			}
		);
	}

	template <typename Tgpio_id>
	inline engine::Task<engine::Data *> *ServerTaskEngine<Tgpio_id>::create_gpio_set_task(const engine::Data& cfg) {
		auto gpio_id = m_gpio_id_parser->parse(cfg);
		auto gpio_state = m_gpio_state_parser->parse(cfg);
		return new engine_utl::FunctionalTask<engine::Data *>(
			[this, gpio_id, gpio_state]()-> engine::Data * {
				auto gpio_ptr = m_gpio_inventory->access(gpio_id);
				Gpio::cast<Gpo>(*gpio_ptr).set_state(gpio_state);
				engine::Object report;
				report.add("result", engine::Integer(0));
				return report.clone();
			}
		);
	}

	template <typename Tgpio_id>
	inline engine::Task<engine::Data *> *ServerTaskEngine<Tgpio_id>::create_gpio_read_task(const engine::Data& cfg) {
		auto gpio_id = m_gpio_id_parser->parse(cfg);
		return new engine_utl::FunctionalTask<engine::Data *>(
			[this, gpio_id]()-> engine::Data * {
				auto gpio_ptr = m_gpio_inventory->access(gpio_id);
				Gpio::State state(Gpio::State::LOW);
				switch (gpio_ptr->direction()) {
				case Gpio::Direction::IN:
					state = Gpio::cast<Gpi>(*gpio_ptr).state();
				case Gpio::Direction::OUT:
					state = Gpio::cast<Gpo>(*gpio_ptr).state();
				default:
					throw std::invalid_argument("unsupported gpio type");
				}
				engine::Object report;
				report.add("result", engine::Integer(0));
				report.add("state", engine::Integer(static_cast<int>(state)));
				return report.clone();
			}
		);
	}


	

	template <typename Tgpio_id>
	inline ServerTaskEngine<Tgpio_id>::ServerTaskEngine(const TaskIdParser& task_id_parser, GpioInventory *gpio_inventory, const GpioCreator& gpio_creator, const GpioIdParser& gpio_id_parser, const GpioDirParser& gpio_dir_parser, const GpioStateParser& gpio_state_parser): m_gpio_inventory(gpio_inventory), m_gpio_creator(gpio_creator.clone()), m_gpio_id_parser(gpio_id_parser.clone()), m_gpio_dir_parser(gpio_dir_parser.clone()), m_gpio_state_parser(gpio_state_parser.clone()), m_task_factory(task_id_parser) {
		if (!m_gpio_inventory) {
			throw std::invalid_argument("invalid gpio_inventory pointer received");
		}
		auto create_gpio_task = InventoryTask<Tgpio_id, Gpio>(
			
		)
		m_task_factory.register_creator(
			TaskId::CREATE_GPIO,
			engine_utl::FunctionalCreator<engine::Task<engine::Data *> *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> engine::Task<engine::Data *> * {
					return create_gpio_create_task(cfg);
				}
			)
		);
		m_task_factory.register_creator(
			TaskId::DELETE_GPIO,
			engine_utl::FunctionalCreator<engine::Task<engine::Data *> *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> engine::Task<engine::Data *> * {
					return create_gpio_delete_task(cfg);
				}
			)
		);
		m_task_factory.register_creator(
			TaskId::SET_GPIO_STATE,
			engine_utl::FunctionalCreator<engine::Task<engine::Data *> *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> engine::Task<engine::Data *> * {
					return create_gpio_set_task(cfg);
				}
			)
		);
		m_task_factory.register_creator(
			TaskId::READ_GPIO_STATE,
			engine_utl::FunctionalCreator<engine::Task<engine::Data *> *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> engine::Task<engine::Data *> * {
					return create_gpio_read_task(cfg);
				}
			)
		);
	}

	template <typename Tgpio_id>
	inline engine::Task<engine::Data *> *ServerTaskEngine<Tgpio_id>::create(const engine::Data& cfg) const {
		return m_task_factory.create(cfg);
	}

	template <typename Tgpio_id>
	inline engine::Creator<engine::Task<engine::Data *> *(const engine::Data&)> *ServerTaskEngine<Tgpio_id>::clone() const {
		return new ServerTaskEngine<Tgpio_id>(*this);
	}
}

#endif // SERVER_TASK_ENGINE_HPP