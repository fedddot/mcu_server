#ifndef MCU_SERVER_FIXTURE_HPP
#define MCU_SERVER_FIXTURE_HPP

#include "creator.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "mcu_factory_fixture.hpp"
#include "object.hpp"
#include "parser.hpp"
#include "serializer.hpp"
#include "string.hpp"
#include <exception>

namespace mcu_server_uts {
	class McuServerFixture: public mcu_factory_uts::McuFactoryFixture {
	public:
		McuServerFixture(const std::string& msg_head = "test_msg_head", const std::string& msg_tail = "test_msg_tail", const std::size_t& max_buffer_size = 1000UL): 
			mcu_factory_uts::McuFactoryFixture(msg_head, msg_tail, max_buffer_size), 
			m_factory(
				platform(),
				task_type_parser(),
				gpio_id_parser(),
				gpio_dir_parser(),
				gpio_state_parser(),
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