#ifndef STEPPER_MOTOR_TASKS_FACTORY_FIXTURE_HPP
#define STEPPER_MOTOR_TASKS_FACTORY_FIXTURE_HPP

#include <string>

#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor_tasks_factory.hpp"
#include "default_stepper_motor_tasks_data_retriever.hpp"
#include "stepper_motor_test_data_creator.hpp"
#include "test_platform.hpp"

namespace mcu_factory_uts {
	class StepperMotorTasksFactoryFixture: public testing::Test {
	public:
		using GpioId = int;
		using StepperId = int;
		using TestFactory = mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>;
		using MotorInventory = typename TestFactory::MotorInventory;
		using GpioState = typename mcu_platform::Gpio::State;
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using DataRetriever = typename TestFactory::DataRetriever;
		using FactoryPlatform = typename TestFactory::FactoryPlatform;
		using ResultReporter = typename TestFactory::ResultReporter;


		using States = typename mcu_platform::StepperMotor<GpioId>::States;
		using Direction = typename mcu_platform::StepperMotor<GpioId>::Direction;
		using Shoulders = typename mcu_platform::StepperMotor<GpioId>::Shoulders;
		using TaskType = typename TestFactory::TaskType;
		using Shoulder = typename mcu_platform::StepperMotor<GpioId>::Shoulder;

		using Steps = typename TestFactory::Steps;
		using StepsSequence = typename TestFactory::StepsSequence;

		StepperMotorTasksFactoryFixture();
		StepperMotorTasksFactoryFixture(const StepperMotorTasksFactoryFixture&) = delete;
		StepperMotorTasksFactoryFixture& operator=(const StepperMotorTasksFactoryFixture&) = delete;
		
		MotorInventory *inventory() const {
			return &m_inventory;
		}

		FactoryPlatform *platform() const {
			return &m_platform;
		}

		const DataRetriever& data_retriever() const {
			return *m_data_retriever;
		}

		const ResultReporter& result_reporter() const {
			return *m_result_reporter;
		}

		const StepperMotorTestDataCreator& test_data_ctor() const {
			return m_test_data_ctor;
		}
	private:
		mutable MotorInventory m_inventory;
		mutable mcu_platform_uts::TestPlatform<GpioId> m_platform;
		std::unique_ptr<DataRetriever> m_data_retriever;
		std::unique_ptr<ResultReporter> m_result_reporter;
		StepperMotorTestDataCreator m_test_data_ctor;
		static std::string shoulder_to_str(const Shoulder& shoulder) {
			const std::string prefix("IN");
			return prefix + std::to_string(static_cast<int>(shoulder));
		}
	};

	inline StepperMotorTasksFactoryFixture::StepperMotorTasksFactoryFixture():
		m_data_retriever(new mcu_factory::DefaultStepperMotorDataRetriever<GpioId, StepperId>()),
		m_result_reporter(
			new server_utl::CustomCreator<server::Data *(int)>(
				[](int result) {
					server::Object report;
					report.add("result", server::Integer(result));
					return report.clone();
				}
			)
		) {
		
	}
}

#endif