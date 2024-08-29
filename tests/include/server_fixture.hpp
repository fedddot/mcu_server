#ifndef MCU_SERVER_FIXTURE_HPP
#define MCU_SERVER_FIXTURE_HPP

#include <exception>
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "data.hpp"
#include "default_gpio_tasks_data_retriever.hpp"
#include "default_stepper_motor_tasks_data_retriever.hpp"
#include "factory.hpp"
#include "gpio.hpp"
#include "gpio_tasks_factory.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "object.hpp"
#include "server.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_tasks_factory.hpp"
#include "string.hpp"
#include "test_platform.hpp"

namespace server_uts {
	class ServerFixture: public testing::Test {
	public:
		using ServerTask = typename server::Server::ServerTask;
		using TaskFactory = typename server::Server::TaskFactory;
		using FailureReportCreator = typename server::Server::FailureReportCreator;

		using GpioId = int;
		using StepperId = int;
		using MotorInventory = mcu_platform::Inventory<StepperId, mcu_platform::StepperMotor<GpioId>>;
		using GpioInventory = mcu_platform::Inventory<GpioId, mcu_platform::Gpio>;
		using Shoulder = typename mcu_platform::StepperMotor<GpioId>::Shoulder;
		using Shoulders = typename mcu_platform::StepperMotor<GpioId>::Shoulders;
		using MotorDirection = typename mcu_platform::StepperMotor<GpioId>::Direction;
		using State = typename mcu_platform::StepperMotor<GpioId>::State;
		using States = typename mcu_platform::StepperMotor<GpioId>::States;
		using Steps = typename mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>::Steps;
		using StepsSequence = typename mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>::StepsSequence;
		
		ServerFixture() = default;
		ServerFixture(const ServerFixture& other) = delete;
		ServerFixture& operator=(const ServerFixture& other) = delete;

		void SetUp() override {
			using namespace server;
			using namespace server_utl;
			m_fail_report_creator = std::unique_ptr<FailureReportCreator>(
				new CustomCreator<Data *(const std::exception&)>(
					[](const std::exception& e) {
						Object data;
						data.add("result", Integer(-1));
						data.add("what", String(std::string(e.what())));
						return data.clone();
					}
				)
			);
			CustomCreator<Data *(int)> result_reporter(
				[](int result) {
					Object report;
					report.add("result", Integer(result));
					return report.clone();
				}
			);
			CustomCreator<Data *(int, const mcu_platform::Gpio::State&)> result_state_reporter(
				[](int result, const mcu_platform::Gpio::State& state) {
					Object report;
					report.add("result", Integer(result));
					report.add("gpio_state", Integer(static_cast<int>(state)));
					return report.clone();
				}
			);
			m_factory.add_task_creator(
				mcu_factory::GpioTasksFactory<GpioId>(
					&m_gpio_inventory,
					&m_platform,
					mcu_factory::DefaultGpioTasksDataRetriever<GpioId>(),
					result_reporter,
					result_state_reporter
				)
			);
			m_factory.add_task_creator(
				mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>(
					&m_motor_inventory,
					&m_platform,
					mcu_factory::DefaultStepperMotorDataRetriever<GpioId, StepperId>(),
					result_reporter
				)
			);
		}
	
		const TaskFactory& factory() const {
			return m_factory;
		}

		const FailureReportCreator& fail_report_creator() const {
			return *m_fail_report_creator;
		}

		MotorInventory *motor_inventory() const {
			return &m_motor_inventory;
		}

		GpioInventory *gpio_inventory() const {
			return &m_gpio_inventory;
		}
	private:
		mutable MotorInventory m_motor_inventory;
		mutable GpioInventory m_gpio_inventory;
		mutable mcu_factory::Factory m_factory;
		std::unique_ptr<FailureReportCreator> m_fail_report_creator;
		mcu_platform_uts::TestPlatform<GpioId> m_platform;
	};
}

#endif