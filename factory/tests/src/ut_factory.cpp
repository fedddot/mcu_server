#include "gtest/gtest.h"
#include "factory_fixture.hpp"
#include "mcu_factory.hpp"

using namespace server;
using namespace server_utl;
using namespace mcu_factory;
using namespace mcu_factory_uts;

TEST_F(StepperMotorTasksFactoryFixture, ctor_dtor_sanity) {
	// WHEN
	TestFactory *instance_ptr(nullptr);
	TestFactory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestFactory(
				inventory(),
				platform(),
				data_retriever(),
				result_reporter()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(
		(
			instance_ptr_copy = new TestFactory(*instance_ptr)
		)
	);
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}

TEST_F(StepperMotorTasksFactoryFixture, create_sanity) {
	// GIVEN
	using GpoState = typename mcu_platform::Gpio::State;
	Shoulders shoulders {
		{ Shoulder::IN0, 10 },
		{ Shoulder::IN1, 11 },
		{ Shoulder::IN2, 12 },
		{ Shoulder::IN3, 13 }
	};
	States states {
		{
			{ Shoulder::IN0, GpoState::HIGH },
			{ Shoulder::IN1, GpoState::LOW },
			{ Shoulder::IN2, GpoState::LOW },
			{ Shoulder::IN3, GpoState::LOW }
		},
		{
			{ Shoulder::IN0, GpoState::LOW },
			{ Shoulder::IN1, GpoState::LOW },
			{ Shoulder::IN2, GpoState::HIGH },
			{ Shoulder::IN3, GpoState::LOW }
		}
	};
	const StepperId id(12);
	const unsigned int cw_steps_num(45);
	const unsigned int cw_step_duration(1);
	const unsigned int ccw_steps_num(50);
	const unsigned int ccw_step_duration(2);

	// WHEN
	TestFactory instance(
		inventory(),
		platform(),
		data_retriever(),
		result_reporter()
	);

	using Task = Task<Data *(void)>;
	std::unique_ptr<Task> task_ptr(nullptr);
	std::unique_ptr<Data> report_ptr(nullptr);

	// THEN
	// Motor creation
	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(create_data(id, shoulders, states))));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
	ASSERT_NE(nullptr, report_ptr);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());
	ASSERT_TRUE(inventory()->contains(id));

	// CCW motion
	Steps ccw_steps {
		.stepper_id = id, 
		.direction = Direction::CCW, 
		.steps_number = ccw_steps_num, 
		.step_duration_ms = ccw_step_duration
	};
	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(steps_data(ccw_steps))));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
	ASSERT_NE(nullptr, report_ptr);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());

	// CW motion
	Steps cw_steps {
		.stepper_id = id, 
		.direction = Direction::CW, 
		.steps_number = cw_steps_num, 
		.step_duration_ms = cw_step_duration
	};
	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(steps_data(cw_steps))));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
	ASSERT_NE(nullptr, report_ptr);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());

	// Motor deletion
	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(delete_data(id))));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
	ASSERT_NE(nullptr, report_ptr);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());
	ASSERT_FALSE(inventory()->contains(id));
}

// TEST_F(StepperMotorTasksFactoryFixture, create_steps_sequence_task_sanity) {
// 	// GIVEN	
// 	Shoulders shoulders_x {
// 		{ Shoulder::IN0, 10 },
// 		{ Shoulder::IN1, 11 },
// 		{ Shoulder::IN2, 12 },
// 		{ Shoulder::IN3, 13 }
// 	};
// 	Shoulders shoulders_y {
// 		{ Shoulder::IN0, 14 },
// 		{ Shoulder::IN1, 15 },
// 		{ Shoulder::IN2, 16 },
// 		{ Shoulder::IN3, 17 }
// 	};
// 	States states {
// 		{
// 			{ Shoulder::IN0, GpoState::HIGH },
// 			{ Shoulder::IN1, GpoState::LOW },
// 			{ Shoulder::IN2, GpoState::LOW },
// 			{ Shoulder::IN3, GpoState::LOW }
// 		},
// 		{
// 			{ Shoulder::IN0, GpoState::LOW },
// 			{ Shoulder::IN1, GpoState::LOW },
// 			{ Shoulder::IN2, GpoState::HIGH },
// 			{ Shoulder::IN3, GpoState::LOW }
// 		}
// 	};
// 	const StepperId id_x(0);
// 	const StepperId id_y(1);

// 	// WHEN
// 	TestFactory instance(
// 		inventory(),
// 		data_parser(),
// 		CustomCreator<mcu_platform::Gpo *(const GpioId&)>(
// 			[](const GpioId& gpio_id) {
// 				return new mcu_platform_uts::TestGpo();
// 			}
// 		),
// 		mcu_platform_uts::TestDelay(),
// 		CustomCreator<Data *(int)>(
// 			[](int result) {
// 				Object report;
// 				report.add("result", Integer(result));
// 				return report.clone();
// 			} 
// 		)
// 	);
// 	StepsSequence sequence {
// 		Steps(id_x, Direction::CCW, 40, 1),
// 		Steps(id_y, Direction::CCW, 30, 2),
// 		Steps(id_x, Direction::CW, 40, 1),
// 		Steps(id_y, Direction::CW, 30, 2)
// 	};
	
// 	using Task = Task<Data *(void)>;
// 	std::unique_ptr<Task> task_ptr(nullptr);
// 	std::unique_ptr<Data> report_ptr(nullptr);

// 	// THEN
// 	// Motor X creation
// 	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(create_data(id_x, shoulders_x, states))));
// 	ASSERT_NE(nullptr, task_ptr);
// 	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
// 	ASSERT_NE(nullptr, report_ptr);
// 	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());
// 	ASSERT_TRUE(inventory()->contains(id_x));
	
// 	// Motor Y creation
// 	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(create_data(id_y, shoulders_y, states))));
// 	ASSERT_NE(nullptr, task_ptr);
// 	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
// 	ASSERT_NE(nullptr, report_ptr);
// 	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());
// 	ASSERT_TRUE(inventory()->contains(id_y));

// 	// Sequence motion
// 	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(steps_sequence_data(sequence))));
// 	ASSERT_NE(nullptr, task_ptr);
// 	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
// 	ASSERT_NE(nullptr, report_ptr);
// 	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());

// 	// Motor X deletion
// 	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(delete_data(id_x))));
// 	ASSERT_NE(nullptr, task_ptr);
// 	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
// 	ASSERT_NE(nullptr, report_ptr);
// 	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());
// 	ASSERT_FALSE(inventory()->contains(id_x));

// 	// Motor Y deletion
// 	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task>(instance.create(delete_data(id_y))));
// 	ASSERT_NE(nullptr, task_ptr);
// 	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
// 	ASSERT_NE(nullptr, report_ptr);
// 	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());
// 	ASSERT_FALSE(inventory()->contains(id_y));
// }