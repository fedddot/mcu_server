#ifndef TASKS_FIXTURE_HPP
#define TASKS_FIXTURE_HPP

#include <memory>

#include "creator.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "mcu_factory_fixture.hpp"
#include "object.hpp"

namespace mcu_factory_uts {
	class TasksFixture: public McuFactoryFixture {
	public:
		using TaskReportCreator = server::Creator<server::Data *(int)>;
		TasksFixture():
			m_factory(
				platform(),
				parsers(),
				result_reporter(),
				result_state_reporter(),
				tasks_results_reporter()
			),
			m_report_ctor(
				new server_utl::CustomCreator<server::Data *(int)>(
					[](int result) {
						server::Object report;
						report.add("result", server::Integer(result));
						return report.clone();
					}
				)
			) {
		
		}
		TasksFixture(const TasksFixture&) = delete;
		TasksFixture& operator=(const TasksFixture&) = delete;

		const TestFactory& factory() const {
			return m_factory;
		}

		const TaskReportCreator& task_id_ctor() const {
			return *m_report_ctor;
		}
		
	private:
		TestFactory m_factory;
		std::unique_ptr<TaskReportCreator> m_report_ctor;
	};

}

#endif