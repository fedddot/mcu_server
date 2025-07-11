#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "manager_instance.hpp"
#include "relay_controller.hpp"
#include "temperature_sensor_controller.hpp"
#include "thermostat_manager.hpp"
#include "timer_scheduler.hpp"

using namespace manager;
using ::testing::NiceMock;

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
    ASSERT_NO_THROW(instance = new ThermostatManager(relay, temp, timer));
    ASSERT_NE(instance, nullptr);
    ASSERT_NO_THROW(delete instance);
}