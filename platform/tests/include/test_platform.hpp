#ifndef	TEST_PLATFORM_HPP
#define	TEST_PLATFORM_HPP

#include <chrono>
#include <stdexcept>
#include <thread>

#include "gpio.hpp"
#include "platform.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"


namespace mcu_platform_uts {
	
	template <typename Tgpio_id>
	class TestPlatform: public mcu_platform::Platform<Tgpio_id> {
	public:
		TestPlatform() = default;
		TestPlatform(const TestPlatform&) = delete;
		TestPlatform& operator=(const TestPlatform&) = delete;
		
		void delay(const unsigned int delay_ms) const override {
			std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
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
	};
}
#endif // TEST_PLATFORM_HPP