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
#include "server.hpp"
#include "object.hpp"
#include "parser.hpp"
#include "serializer.hpp"
#include "string.hpp"
#include "task.hpp"

namespace server_uts {
	class ServerFixture: public mcu_factory_uts::McuFactoryFixture {
	public:
		using ServerTask = typename server::Server::ServerTask;
		
		ServerFixture():
			m_factory(
				platform(),
				parsers(),
				result_reporter(),
				result_state_reporter(),
				tasks_results_reporter()
			),
			m_fail_report_creator(
				[](const std::exception& e) {
					server::Object report;
					report.add("result", server::Integer(-1));
					report.add("what", server::String(e.what()));
					return report.clone();
				}
			) {
		
		}
		ServerFixture(const ServerFixture& other) = delete;
		ServerFixture& operator=(const ServerFixture& other) = delete;
	
		const TestFactory& factory() const {
			return m_factory;
		}

		const server::Creator<server::Data *(const std::exception&)>& fail_report_creator() const {
			return m_fail_report_creator;
		}

		const server::Parser<server::Data *(const McuData&)>& parser() const {
			return m_parser;
		}

		const server::Serializer<McuData(const server::Data&)>& serializer() const {
			return m_serializer;
		}
	private:
		TestFactory m_factory;
		server_utl::CustomCreator<server::Data *(const std::exception&)> m_fail_report_creator;
		server_utl::JsonDataParser m_parser;
		server_utl::JsonDataSerializer m_serializer;
	};
}

#endif