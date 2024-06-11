#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"

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

TEST(ut_engine, run_task_sanity_negative) {
	// GIVEN
	static const int fail_code(-100);
	static const int success_code(0);

	static FunctionalCreator<EngineTask *(const Data&)> task_creator(
		[](const Data& data)-> EngineTask * {
			auto cfg_obj = Data::cast<Object>(data);
			auto test_return_value = static_cast<TestReport>(Data::cast<Integer>(cfg_obj.access("test_return_value")).get()); 
			return new FunctionalTask<Data *>(
				[test_return_value](void)-> Data * {
					Object report;
					report.add("result", Integer(test_return_value));
					return report.clone();
				}
			);
		}
	);


	FunctionalCreator<Data *(const std::exception&)> fail_report_creator(
		[](const std::exception& e)-> Data * {
			Object report;
			report.add("result", Integer(fail_code));
			return report.clone();
		}
	);
	FunctionalParser<Data *(const TestRawData&)> parser(
		[](const TestRawData& data) -> Data * {
			const std::string delimiter("=");
			auto delimiter_pos = data.find(delimiter);
			const std::string key(data.substr(0, delimiter_pos));
			const std::string val(data.substr(delimiter_pos + 1, data.size() - delimiter_pos));
			Object parsed;
			parsed.add(key, Integer(std::stoi(val)));
			return parsed.clone();
		}
	);
	FunctionalSerializer<TestReport(const Data&)> serializer(
		[](const Data& data) -> TestReport {
			return Data::cast<Integer>(Data::cast<Object>(data).access("result")).get();
		}
	);
	const std::map<TestRawData, TestReport> test_cases {
		{"test_return_value=" + std::to_string(fail_code), fail_code},
		{"test_return_value=" + std::to_string(success_code), success_code},
		{"test_return_value=gaga", fail_code},
		{"gaga=" + std::to_string(success_code), fail_code}
	};

	// WHEN
	Engine<TestRawData, TestReport> instance(task_creator, fail_report_creator, parser, serializer);

	for (auto test_case: test_cases) {
		// WHEN
		TestReport report(fail_code);

		// THEN
		std::cout << "running TC with input: " << test_case.first << std::endl;
		ASSERT_NO_THROW(
			(report = instance.run_task(test_case.first))
		);

		std::cout << '\t' << "received report: " << std::to_string(report) << std::endl;
		std::cout << '\t' << "expected report: " << std::to_string(test_case.second) << std::endl;
		
		ASSERT_EQ(test_case.second, report);
	}
}