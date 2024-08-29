#include <memory>

#include "gtest/gtest.h"

#include "custom_task.hpp"
#include "data.hpp"
#include "factory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "string.hpp"
#include "task.hpp"
#include "test_task_creator.hpp"

using namespace server;
using namespace server_utl;
using namespace mcu_factory;
using namespace mcu_factory_uts;

TEST(ut_factory, ctor_dtor_sanity) {
	// WHEN
	Factory *instance_ptr(nullptr);
	Factory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new Factory());
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(
		(
			instance_ptr_copy = new Factory(*instance_ptr)
		)
	);
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}

void run_create_sanity_tc(Factory *instance_ptr, const Data& test_data, const std::string& expected_domain, const int expected_result) {
	// WHEN
	using TestTask = Task<Data *(void)>;
	std::unique_ptr<TestTask> task_ptr(nullptr);
	std::unique_ptr<Data> report_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(task_ptr = std::unique_ptr<TestTask>(instance_ptr->create(test_data)));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
	ASSERT_NE(nullptr, report_ptr);
	ASSERT_EQ(expected_domain, Data::cast<String>(Data::cast<Object>(*report_ptr).access("domain")).get());
	ASSERT_EQ(expected_result, Data::cast<Integer>(Data::cast<Object>(*report_ptr).access("result")).get());
}

TEST(ut_factory, create_sanity) {
	// GIVEN
	const auto generate_test_data = [](const std::string& domain, const int expected_result) {
		Object data;
		data.add("domain", String(domain));
		data.add("expected_result", Integer(expected_result));
		return data;
	};
	const std::string test_domain1("domain1");
	const int expected_result1(12);
	const std::string test_domain2("domain2");
	const int expected_result2(13);
	const TestTaskCreator ctor1(
		[](const Data& data) {
			Object report;
			report.add("result", Data::cast<Object>(data).access("expected_result"));
			report.add("domain", Data::cast<Object>(data).access("domain"));
			return new CustomTask<Data *(void)>(
				[report](void) {
					return report.clone();
				}
			);
		},
		[test_domain1](const Data& data) {
			return test_domain1 == Data::cast<String>(Data::cast<Object>(data).access("domain")).get();
		}
	);
	const TestTaskCreator ctor2(
		[](const Data& data) {
			Object report;
			report.add("result", Data::cast<Object>(data).access("expected_result"));
			report.add("domain", Data::cast<Object>(data).access("domain"));
			return new CustomTask<Data *(void)>(
				[report](void) {
					return report.clone();
				}
			);
		},
		[test_domain2](const Data& data) {
			return test_domain2 == Data::cast<String>(Data::cast<Object>(data).access("domain")).get();
		}
	);

	// WHEN
	Factory instance;
	instance.add_task_creator(ctor1);
	instance.add_task_creator(ctor2);
	
	// THEN
	run_create_sanity_tc(&instance, generate_test_data(test_domain1, expected_result1), test_domain1, expected_result1);
	run_create_sanity_tc(&instance, generate_test_data(test_domain2, expected_result2), test_domain2, expected_result2);
}