#ifndef	TEST_DELAY_HPP
#define	TEST_DELAY_HPP

#include <chrono>
#include <thread>

#include "delay.hpp"

namespace mcu_platform_uts {
	class TestDelay: public mcu_platform::Delay {
	public:
		TestDelay() = default;
		TestDelay(const TestDelay&) = default;
		TestDelay& operator=(const TestDelay&) = default;

		void delay(int delay_ms) const override {
			std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
		}
		mcu_platform::Delay *clone() const override {
			return new TestDelay(*this);
		}
	};
}
#endif // TEST_DELAY_HPP