#include <atomic>
#include <iostream>
#include <optional>
#include <thread>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "manager_instance.hpp"
#include "thermostat_manager_controller.hpp"
#include "thermostat_manager.hpp"

using namespace manager;
using namespace testing;

class MockThermostatManagerController : public ThermostatManagerController {
public:
    MOCK_METHOD(void, set_relay_state, (const bool state), (override));
	MOCK_METHOD(double, read_temperature, (), (const, override));
	MOCK_METHOD(Instance<TaskGuard>, schedule_task, (const Task& task, const std::size_t period_ms), (override));
};

class MockTaskGuard : public ThermostatManagerController::TaskGuard {
	public:
    MOCK_METHOD(void, unschedule, (), (override));
};

TEST(ut_thermostat_manager, ctor_dtor_sanity) {
    // GIVEN
    auto controller = NiceMock<MockThermostatManagerController>();

    // WHEN
    ThermostatManager* instance = nullptr;

	// THEN
    ASSERT_NO_THROW(instance = new ThermostatManager(&controller));
    ASSERT_NE(instance, nullptr);
    ASSERT_NO_THROW(delete instance);
}

TEST(ut_thermostat_manager, start_stop_sanity) {
    // GIVEN
    auto controller = NiceMock<MockThermostatManagerController>();
    auto guard = new NiceMock<MockTaskGuard>();
	
	// WHEN
	EXPECT_CALL(controller, set_relay_state)
		.WillRepeatedly(
			[](const bool state) {
				std::cout << "Relay state set to: " << (state ? "ON" : "OFF") << std::endl;
			}
		);
    EXPECT_CALL(controller, read_temperature()).WillRepeatedly(Return(22.5));
	
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
    EXPECT_CALL(controller, schedule_task)
		.WillOnce(
			[&thread, &is_running, &guard](const ThermostatManagerController::Task& task, const std::size_t period_ms) {
				is_running = true;
				thread = std::make_optional<std::thread>([task, period_ms, &is_running]() {
					while (is_running) {
						task();
						std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
					}
				});
				return Instance<ThermostatManagerController::TaskGuard>(guard);
			}
		);

    ThermostatManager instance(&controller);

    // WHEN/THEN
    ASSERT_NO_THROW(instance.start(23.0, 100));
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
    ASSERT_NO_THROW(instance.stop());
}