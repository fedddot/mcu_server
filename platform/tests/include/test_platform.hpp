#ifndef	TEST_PLATFORM_HPP
#define	TEST_PLATFORM_HPP

#include <stdexcept>
#include <string>

#include "delay.hpp"
#include "gpio.hpp"
#include "platform.hpp"
#include "test_delay.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"


namespace mcu_platform_uts {
	
	using TestPlatformGpioId = int;
	using TestPlatformTaskId = std::string;

	class TestPlatform: public mcu_platform::Platform<TestPlatformGpioId, TestPlatformTaskId> {
	public:
		TestPlatform() = default;
		TestPlatform(const TestPlatform&) = delete;
		TestPlatform& operator=(const TestPlatform&) = delete;
		
		mcu_platform::Delay *create_delay() const override {
			return new TestDelay();
		}
		
		mcu_platform::Gpio *create_gpio(const TestPlatformGpioId& id, const mcu_platform::Gpio::Direction& dir) const override {
			switch (dir) {
			case mcu_platform::Gpio::Direction::IN:
				return new mcu_platform_uts::TestGpi();
			case mcu_platform::Gpio::Direction::OUT:
				return new mcu_platform_uts::TestGpo();
			default:
				throw std::invalid_argument("unsupported GPIO direction received");
			}
		}

		mcu_platform::Inventory<TestPlatformGpioId, mcu_platform::Gpio> *gpio_inventory() const override {
			return &m_gpio_inventory;
		}

		mcu_platform::Inventory<TestPlatformTaskId, PersistentTask> *task_inventory() const override {
			return &m_task_inventory;
		}
	private:
		mutable mcu_platform::Inventory<TestPlatformGpioId, mcu_platform::Gpio> m_gpio_inventory;
		mutable mcu_platform::Inventory<TestPlatformTaskId, PersistentTask> m_task_inventory;
	};
}
#endif // TEST_PLATFORM_HPP