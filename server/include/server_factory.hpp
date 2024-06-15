#ifndef	SERVER_FACTORY_HPP
#define	SERVER_FACTORY_HPP

#include "creator.hpp"
#include "data.hpp"
#include "functional_creator.hpp"
#include "functional_task.hpp"
#include "gpi.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "object.hpp"
#include "parser.hpp"
#include "task.hpp"
#include "task_factory.hpp"

#include <memory>
#include <stdexcept>

namespace server {
	template <typename Tgpio_id>
	class ServerFactory: public engine::Creator<engine::Task<engine::Data *> *(const engine::Data&)> {
	public:
		enum class TaskId: int {
			CREATE_GPIO,
			DELETE_GPIO,
			SET_GPIO_STATE,
			READ_GPIO_STATE
		};
		using TaskIdParser = engine::Parser<TaskId(const engine::Data&)>;
		
		using GpioInventory = Inventory<Tgpio_id, Gpio>;
		using GpioCreator = engine::Creator<Gpio *(const Tgpio_id&, const Gpio::Direction&)>;
		using GpioIdParser = engine::Parser<Tgpio_id(const engine::Data&)>;
		using GpioDirParser = engine::Parser<Gpio::Direction(const engine::Data&)>;
		using GpioStateParser = engine::Parser<Gpio::State(const engine::Data&)>;
		
		ServerFactory(const TaskIdParser& task_id_parser, GpioInventory *gpio_inventory, const GpioCreator& gpio_creator, const GpioIdParser& gpio_id_parser, const GpioDirParser& gpio_dir_parser, const GpioStateParser& gpio_state_parser);
		ServerFactory(const ServerFactory& other) = default;
		ServerFactory& operator=(const ServerFactory& other) = default;

		engine::Task<engine::Data *> *create(const engine::Data& cfg) const override;
		engine::Creator<engine::Task<engine::Data *> *(const engine::Data&)> *clone() const override;
	private:
		GpioInventory *m_gpio_inventory;
		std::shared_ptr<GpioCreator> m_gpio_creator;
		std::shared_ptr<GpioIdParser> m_gpio_id_parser;
		std::shared_ptr<GpioDirParser> m_gpio_dir_parser;
		std::shared_ptr<GpioStateParser> m_gpio_state_parser;

		server_utl::TaskFactory<engine::Data, engine::Data *, TaskId> m_task_factory;
	};

	template <typename Tgpio_id>
	inline ServerFactory<Tgpio_id>::ServerFactory(const TaskIdParser& task_id_parser, GpioInventory *gpio_inventory, const GpioCreator& gpio_creator, const GpioIdParser& gpio_id_parser, const GpioDirParser& gpio_dir_parser, const GpioStateParser& gpio_state_parser): m_gpio_inventory(gpio_inventory), m_gpio_creator(gpio_creator.clone()), m_gpio_id_parser(gpio_id_parser.clone()), m_gpio_dir_parser(gpio_dir_parser.clone()), m_gpio_state_parser(gpio_state_parser.clone()), m_task_factory(task_id_parser) {
		if (!m_gpio_inventory) {
			throw std::invalid_argument("invalid gpio_inventory pointer received");
		}
		m_task_factory.register_creator(
			TaskId::CREATE_GPIO,
			engine_utl::FunctionalCreator<engine::Task<engine::Data *> *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> engine::Task<engine::Data *> * {
					return new engine_utl::FunctionalTask<engine::Data *>(
						[this, &cfg]()-> engine::Data * {
							m_gpio_inventory->put(
								m_gpio_id_parser->parse(cfg),
								m_gpio_creator->create(m_gpio_id_parser->parse(cfg), m_gpio_dir_parser->parse(cfg))
							);
							engine::Object report;
							report.add("result", engine::Integer(0));
							return report.clone();
						}
					);
				}
			)
		);
		m_task_factory.register_creator(
			TaskId::DELETE_GPIO,
			engine_utl::FunctionalCreator<engine::Task<engine::Data *> *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> engine::Task<engine::Data *> * {
					return new engine_utl::FunctionalTask<engine::Data *>(
						[this, &cfg]()-> engine::Data * {
							delete m_gpio_inventory->pull(m_gpio_id_parser->parse(cfg));
							engine::Object report;
							report.add("result", engine::Integer(0));
							return report.clone();
						}
					);
				}
			)
		);
		m_task_factory.register_creator(
			TaskId::SET_GPIO_STATE,
			engine_utl::FunctionalCreator<engine::Task<engine::Data *> *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> engine::Task<engine::Data *> * {
					return new engine_utl::FunctionalTask<engine::Data *>(
						[this, &cfg]()-> engine::Data * {
							auto gpio_ptr = m_gpio_inventory->access(m_gpio_id_parser->parse(cfg));
							Gpio::cast<Gpo>(*gpio_ptr).set_state(m_gpio_state_parser->parse(cfg));
							engine::Object report;
							report.add("result", engine::Integer(0));
							return report.clone();
						}
					);
				}
			)
		);
		m_task_factory.register_creator(
			TaskId::READ_GPIO_STATE,
			engine_utl::FunctionalCreator<engine::Task<engine::Data *> *(const engine::Data&)>(
				[this](const engine::Data& cfg)-> engine::Task<engine::Data *> * {
					return new engine_utl::FunctionalTask<engine::Data *>(
						[this, &cfg]()-> engine::Data * {
							auto gpio_ptr = m_gpio_inventory->access(m_gpio_id_parser->parse(cfg));
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
			)
		);
	}

	template <typename Tgpio_id>
	inline engine::Task<engine::Data *> *ServerFactory<Tgpio_id>::create(const engine::Data& cfg) const {
		return m_task_factory.create(cfg);
	}

	template <typename Tgpio_id>
	inline engine::Creator<engine::Task<engine::Data *> *(const engine::Data&)> *ServerFactory<Tgpio_id>::clone() const {
		return new ServerFactory<Tgpio_id>(*this);
	}
}

#endif // SERVER_FACTORY_HPP