#ifndef MCU_SERVER_FIXTURE_HPP
#define MCU_SERVER_FIXTURE_HPP

#include <exception>

#include "creator.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "mcu_factory_fixture.hpp"
#include "mcu_server.hpp"
#include "object.hpp"
#include "parser.hpp"
#include "serializer.hpp"
#include "string.hpp"
#include "task.hpp"

namespace mcu_server_uts {
	class McuServerFixture: public mcu_factory_uts::McuFactoryFixture {
	public:
		using McuData = std::string;
		using TestMcuServer = mcu_server::McuServer<McuData>;
		using McuTask = mcu_server::Task<mcu_server::Data *(void)>;
		
		McuServerFixture():
			m_factory(
				platform(),
				task_type_parser(),
				gpio_id_parser(),
				gpio_dir_parser(),
				gpio_state_parser(),
				task_id_parser(),
				task_data_parser(),
				tasks_parser(),
				delay_parser(),
				result_reporter(),
				result_state_reporter(),
				tasks_results_reporter()
			),
			m_fail_report_creator(
				[](const std::exception& e) {
					mcu_server::Object report;
					report.add("result", mcu_server::Integer(-1));
					report.add("what", mcu_server::String(e.what()));
					return report.clone();
				}
			) {
		
		}
		McuServerFixture(const McuServerFixture& other) = delete;
		McuServerFixture& operator=(const McuServerFixture& other) = delete;
	
		const TestFactory& factory() const {
			return m_factory;
		}

		const mcu_server::Creator<mcu_server::Data *(const std::exception&)>& fail_report_creator() const {
			return m_fail_report_creator;
		}

		const mcu_server::Parser<mcu_server::Data *(const McuData&)>& parser() const {
			return m_parser;
		}

		const mcu_server::Serializer<McuData(const mcu_server::Data&)>& serializer() const {
			return m_serializer;
		}
	private:
		TestFactory m_factory;
		mcu_server_utl::CustomCreator<mcu_server::Data *(const std::exception&)> m_fail_report_creator;
		mcu_server_utl::JsonDataParser m_parser;
		mcu_server_utl::JsonDataSerializer m_serializer;
	};
}

#endif