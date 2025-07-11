#include <atomic>
#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <optional>
#include <thread>

#include "manager_instance.hpp"
#include "relay_controller.hpp"
#include "temperature_sensor_controller.hpp"
#include "thermostat_manager.hpp"
#include "timer_scheduler.hpp"

using namespace manager;
using namespace testing;

class MockRelayController : public RelayController {
public:
    MOCK_METHOD(void, set_relay_state, (const bool state), (override));
};

class MockTemperatureSensorController : public TemperatureSensorController {
public:
    MOCK_METHOD(double, read_temperature, (), (const, override));
};

class MockTaskGuard : public TimerScheduler::TaskGuard {
public:
    MOCK_METHOD(void, unschedule, (), (override));
};

class MockTimerScheduler : public TimerScheduler {
public:
    MOCK_METHOD(Instance<TaskGuard>, schedule_task, (const Task& task, const std::size_t period_ms), (override));
};

TEST(ut_thermostat_manager, ctor_dtor_sanity) {
    // GIVEN
    auto relay = Instance<RelayController>(new NiceMock<MockRelayController>());
    auto temp = Instance<TemperatureSensorController>(new NiceMock<MockTemperatureSensorController>());
    auto timer = Instance<TimerScheduler>(new NiceMock<MockTimerScheduler>());

    // WHEN
    ThermostatManager* instance = nullptr;

	// THEN
    ASSERT_NO_THROW(instance = new ThermostatManager(relay, temp, timer));
    ASSERT_NE(instance, nullptr);
    ASSERT_NO_THROW(delete instance);
}

TEST(ut_thermostat_manager, start_stop_sanity) {
    // GIVEN
    auto relay = new NiceMock<MockRelayController>();
    auto temp = new NiceMock<MockTemperatureSensorController>();
    auto guard = new NiceMock<MockTaskGuard>();
    auto timer = new NiceMock<MockTimerScheduler>();
	
	// WHEN
	EXPECT_CALL(*relay, set_relay_state)
		.WillRepeatedly(
			[](const bool state) {
				std::cout << "Relay state set to: " << (state ? "ON" : "OFF") << std::endl;
			}
		);
    EXPECT_CALL(*temp, read_temperature()).WillRepeatedly(Return(22.5));
	
	std::atomic_bool is_running(false);
	std::optional<std::thread> thread(std::nullopt);
    EXPECT_CALL(*guard, unschedule)
		.WillOnce(
			[&thread, &is_running]() {
				is_running = false;
				if (thread) {
					if (thread->joinable()) {
						thread->join();
					}
					thread = std::nullopt;
				}
			}
		);
    EXPECT_CALL(*timer, schedule_task)
		.WillOnce(
			[&thread, &is_running, &guard](const TimerScheduler::Task& task, const std::size_t period_ms) {
				is_running = true;
				thread = std::make_optional<std::thread>([task, period_ms, &is_running]() {
					while (is_running) {
						task();
						std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
					}
				});
				return Instance<TimerScheduler::TaskGuard>(guard);
			}
		);

    auto relay_inst = Instance<RelayController>(relay);
    auto temp_inst = Instance<TemperatureSensorController>(temp);
    auto timer_inst = Instance<TimerScheduler>(timer);

    ThermostatManager instance(relay_inst, temp_inst, timer_inst);

    // WHEN/THEN
    ASSERT_NO_THROW(instance.start(23.0, 100));
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
    ASSERT_NO_THROW(instance.stop());
}