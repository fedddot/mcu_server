#include <exception>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "engine.hpp"
#include "functional_creator.hpp"
#include "functional_parser.hpp"
#include "functional_serializer.hpp"
#include "functional_task.hpp"

using namespace engine;
using namespace engine_utl;


using TestRawData = std::string;
using TestReport = int;

using EngineTask = typename Engine<TestRawData, TestReport>::EngineTask;

TEST(ut_engine, ctor_dtor_sanity) {
	// GIVEN
	
	FunctionalCreator<EngineTask *(const Data&)> task_creator(
		[](const Data& data)-> EngineTask * {
			return new FunctionalTask<Data *>(
				[](void)-> Data * {
					throw std::runtime_error("not needed");
				}
			);
		}
	);
	FunctionalCreator<Data *(const std::exception&)> fail_report_creator(
		[](const std::exception& e)-> Data * {
			(void)e;
			throw std::runtime_error("not needed");
		}
	);
	FunctionalParser<Data *(const TestRawData&)> parser(
		[](const TestRawData& data) -> Data * {
			(void)data;
			throw std::runtime_error("not needed");
		}
	);
	FunctionalSerializer<TestReport(const Data&)> serializer(
		[](const Data& data) -> TestReport {
			(void)data;
			throw std::runtime_error("not needed");
		}
	);

	// WHEN
	Engine<TestRawData, TestReport> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(instance_ptr = new Engine<TestRawData, TestReport>(task_creator, fail_report_creator, parser, serializer))
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

// TEST(ut_engine, run_task_sanity) {
// 	// GIVEN
// 	const TestReport task_report_success("ok");
// 	auto task_action = [task_report_success](void)-> TestReport {
// 		return task_report_success;
// 	};

// 	const TestReport task_report_fail("fail");
// 	auto fail_report_action = [task_report_fail](const std::exception& e)-> TestReport {
// 		(void)e;
// 		return task_report_fail;
// 	};

// 	TestCreator<Task<TestReport> *(const TestRawData&)> task_creator(
// 		[task_action](const TestRawData& data)-> Task<TestReport> * {
// 			if (data < 0) {
// 				throw std::runtime_error("data is less than zero");
// 			}
// 			return new TestTask<TestReport>(task_action);
// 		}
// 	);
// 	TestCreator<TestReport(const std::exception&)> fail_report_creator(fail_report_action);

// 	// WHEN
// 	Engine<TestRawData, TestReport> instance(task_creator, fail_report_creator);
// 	TestReport report("");

// 	// THEN
// 	ASSERT_NO_THROW(
// 		(report = instance.run_task(0))
// 	);
// 	ASSERT_EQ(task_report_success, report);
	
// 	ASSERT_NO_THROW(
// 		(report = instance.run_task(-1))
// 	);
// 	ASSERT_EQ(task_report_fail, report);
// }