#ifndef TASKS_FIXTURE_HPP
#define TASKS_FIXTURE_HPP

#include <memory>

#include "creator.hpp"
#include "custom_creator.hpp"
#include "mcu_factory_fixture.hpp"

namespace mcu_factory_uts {
	class TasksFixture: public McuFactoryFixture {
	public:
		using TaskIdCreator = mcu_server::Creator<int(void)>;
		TasksFixture():
			m_factory(
				platform(),
				parsers(),
				result_reporter(),
				result_state_reporter(),
				tasks_results_reporter()
			),
			m_task_id_ctor(
				new mcu_server_utl::CustomCreator<int(void)>(
					[]() {
						static int counter = 0;
						return counter++;
					}
				)
			) {
		
		}
		TasksFixture(const TasksFixture&) = delete;
		TasksFixture& operator=(const TasksFixture&) = delete;

		const TestFactory& factory() const {
			return m_factory;
		}

		const TaskIdCreator& task_id_ctor() const {
			return *m_task_id_ctor;
		}
		
	private:
		TestFactory m_factory;
		std::unique_ptr<TaskIdCreator> m_task_id_ctor;
	};

}

#endif