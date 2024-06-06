#include <exception>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "engine.hpp"
#include "task.hpp"
#include "test_creator.hpp"
#include "test_task.hpp"

using namespace engine;
using namespace engine_ut;


using TestData = int;
using TestReport = std::string;
using TestTaskId = int;

TEST(ut_engine, ctor_dtor_sanity) {
	// GIVEN
	TestCreator<Task<TestReport> *(const TestData&)> task_creator(
		[](const TestData& data)-> Task<TestReport> * {
			return new TestTask<TestReport>(
				[](void)-> TestReport {
					throw std::runtime_error("not needed");
				}
			);
		}
	);
	TestCreator<TestReport(const std::exception&)> fail_report_creator(
		[](const std::exception& e)-> TestReport {
			(void)e;
			throw std::runtime_error("not needed");
		}
	);

	// WHEN
	Engine<TestData, TestReport> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(instance_ptr = new Engine<TestData, TestReport>(task_creator, fail_report_creator))
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

TEST(ut_engine, run_task_sanity) {
	// GIVEN
	const TestReport task_report_success("ok");
	auto task_action = [task_report_success](void)-> TestReport {
		return task_report_success;
	};

	const TestReport task_report_fail("fail");
	auto fail_report_action = [task_report_fail](const std::exception& e)-> TestReport {
		(void)e;
		return task_report_fail;
	};

	TestCreator<Task<TestReport> *(const TestData&)> task_creator(
		[task_action](const TestData& data)-> Task<TestReport> * {
			if (data < 0) {
				throw std::runtime_error("data is less than zero");
			}
			return new TestTask<TestReport>(task_action);
		}
	);
	TestCreator<TestReport(const std::exception&)> fail_report_creator(fail_report_action);

	// WHEN
	Engine<TestData, TestReport> instance(task_creator, fail_report_creator);
	TestReport report("");

	// THEN
	ASSERT_NO_THROW(
		(report = instance.run_task(0))
	);
	ASSERT_EQ(task_report_success, report);
	
	ASSERT_NO_THROW(
		(report = instance.run_task(-1))
	);
	ASSERT_EQ(task_report_fail, report);
}