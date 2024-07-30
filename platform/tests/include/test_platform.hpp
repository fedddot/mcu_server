#ifndef	TEST_PLATFORM_HPP
#define	TEST_PLATFORM_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <thread>

#include "buffered_message_receiver.hpp"
#include "custom_sender.hpp"
#include "gpio.hpp"
#include "platform.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"


namespace mcu_platform_uts {
	
	using TestPlatformData = std::string;
	using TestPlatformGpioId = int;

	class TestPlatform: public mcu_platform::Platform<TestPlatformData, TestPlatformGpioId> {
	public:
		TestPlatform(const std::string& msg_head, const std::string& msg_tail, const std::size_t& max_buffer_size): m_receiver(msg_head, msg_tail, max_buffer_size), m_sender([](const TestPlatformData&) { throw std::runtime_error("SEND ACTION NOT SET"); }) {

		}

		mcu_platform::MessageReceiver<TestPlatformData> *message_receiver() const override {
			return &m_receiver;
		}
		
		mcu_platform::MessageSender<TestPlatformData> *message_sender() const override {
			return &m_sender;
		}

		void delay(unsigned int timeout_ms) const override {
			std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
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

		void set_sender(const std::function<void(const TestPlatformData&)>& send_function) {
			m_sender = mcu_platform_utl::CustomSender<TestPlatformData>(send_function);
		}
	private:
		mutable mcu_platform_utl::BufferedReceiver m_receiver;
		mutable mcu_platform_utl::CustomSender<TestPlatformData> m_sender;
		mutable mcu_platform::Inventory<TestPlatformGpioId, mcu_platform::Gpio> m_gpio_inventory;
	};
}
#endif // TEST_PLATFORM_HPP