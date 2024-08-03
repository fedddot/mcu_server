#ifndef	TEST_PLATFORM_HPP
#define	TEST_PLATFORM_HPP

#include <stdexcept>

#include "delay.hpp"
#include "gpio.hpp"
#include "platform.hpp"
#include "test_delay.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"


namespace mcu_platform_uts {
	
	template <typename Tgpio_id, typename Ttask_id>
	class TestPlatform: public mcu_platform::Platform<Tgpio_id, Ttask_id> {
	public:
		using PersistentTask = typename mcu_platform::Platform<Tgpio_id, Ttask_id>::PersistentTask;
		
		TestPlatform() = default;
		TestPlatform(const TestPlatform&) = delete;
		TestPlatform& operator=(const TestPlatform&) = delete;
		
		mcu_platform::Delay *create_delay() const override {
			return new TestDelay();
		}
		
		mcu_platform::Gpio *create_gpio(const Tgpio_id& id, const mcu_platform::Gpio::Direction& dir) const override {
			switch (dir) {
			case mcu_platform::Gpio::Direction::IN:
				return new mcu_platform_uts::TestGpi();
			case mcu_platform::Gpio::Direction::OUT:
				return new mcu_platform_uts::TestGpo();
			default:
				throw std::invalid_argument("unsupported GPIO direction received");
			}
		}

		mcu_platform::Inventory<Tgpio_id, mcu_platform::Gpio> *gpio_inventory() const override {
			return &m_gpio_inventory;
		}

		mcu_platform::Inventory<Ttask_id, PersistentTask> *task_inventory() const override {
			return &m_task_inventory;
		}
	private:
		mutable mcu_platform::Inventory<Tgpio_id, mcu_platform::Gpio> m_gpio_inventory;
		mutable mcu_platform::Inventory<Ttask_id, PersistentTask> m_task_inventory;
	};
}
#endif // TEST_PLATFORM_HPP