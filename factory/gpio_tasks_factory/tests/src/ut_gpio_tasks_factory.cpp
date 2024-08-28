#include "gtest/gtest.h"
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "data.hpp"
#include "gpio.hpp"
#include "gpio_tasks_factory.hpp"
#include "gpio_tasks_factory_fixture.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "object.hpp"

using namespace server;
using namespace server_utl;
using namespace mcu_factory;
using namespace mcu_factory_uts;
using namespace mcu_platform;

TEST_F(GpioTasksFactoryFixture, ctor_dtor_sanity) {
	// WHEN
	TestFactory *instance_ptr(nullptr);
	TestFactory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestFactory(
				inventory(),
				platform(),
				retriever(),
				result_reporter(),
				result_state_reporter()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(instance_ptr_copy = new GpioTasksFactory<int>(*instance_ptr));
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}

void run_sanity_test_case(const std::string& title, const Data& data, const GpioTasksFactoryFixture *fixture, const std::function<void(const Data&)>& result_assertion) {
	// WHEN
	auto instance_ptr = std::make_unique<GpioTasksFactoryFixture::TestFactory>(
		fixture->inventory(),
		fixture->platform(),
		fixture->retriever(),
		fixture->result_reporter(),
		fixture->result_state_reporter()
	);
	std::unique_ptr<GpioTasksFactoryFixture::TestFactory::GpioTask> task_ptr(nullptr);
	std::unique_ptr<Data> report_ptr(nullptr);

	// THEN
	std::cout << "running TC: " << title << std::endl;
	ASSERT_TRUE(instance_ptr->is_creatable(data));
	ASSERT_NO_THROW(task_ptr = std::unique_ptr<GpioTasksFactoryFixture::TestFactory::GpioTask>(instance_ptr->create(data)));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(instance_ptr = nullptr); // the task should be properly executed even though the creator is destructed
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
	ASSERT_NE(nullptr, report_ptr);
	result_assertion(*report_ptr);
} 

TEST_F(GpioTasksFactoryFixture, create_sanity) {
	// GIVEN
	const GpioId test_gpio_id(12);
	const GpioDirection test_gpio_dir(GpioDirection::OUT);
	const std::vector<std::pair<std::string, std::pair<Object, std::function<void(const Data&)>>>> test_cases {
		{
			"gpio creation",
			{
				test_data_ctor().create_gpio_data(test_gpio_id, test_gpio_dir),
				[this, test_gpio_id](const Data& data) {
					ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(data).access("result")).get());
					ASSERT_TRUE(inventory()->contains(test_gpio_id));
				}
			}
		},
		{
			"gpo set high",
			{
				test_data_ctor().set_gpio_data(test_gpio_id, GpioState::HIGH),
				[this, test_gpio_id](const Data& data) {
					ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(data).access("result")).get());
					ASSERT_EQ(GpioState::HIGH, Gpio::cast<Gpo>(*(inventory()->access(test_gpio_id))).state());
				}
			}
		},
		{
			"gpo get",
			{
				test_data_ctor().get_gpio_data(test_gpio_id),
				[this, test_gpio_id](const Data& data) {
					ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(data).access("result")).get());
					ASSERT_EQ(GpioState::HIGH, static_cast<GpioState>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get()));
				}
			}
		},
		{
			"gpio deletion",
			{
				test_data_ctor().delete_gpio_data(test_gpio_id),
				[this, test_gpio_id](const Data& data) {
					ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(data).access("result")).get());
					ASSERT_FALSE(inventory()->contains(test_gpio_id));
				}
			}
		}
	};

	for (const auto& [title, tc_data]: test_cases) {
		run_sanity_test_case(title, tc_data.first, this, tc_data.second);
	}
}