#ifndef MCU_SERVER_FIXTURE_HPP
#define MCU_SERVER_FIXTURE_HPP

#include <memory>

#include "custom_receiver.hpp"
#include "data.hpp"
#include "data_receiver.hpp"
#include "engine.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "mcu_server.hpp"
#include "mcu_task_engine.hpp"
#include "mcu_task_engine_fixture.hpp"
#include "parser.hpp"
#include "serializer.hpp"

namespace mcu_server_uts {

	class McuServerFixture: public mcu_task_engine_uts::McuTaskEngineFixture {
	public:
		using McuEngine = engine::Engine<engine::Data *(const engine::Data&)>;
		McuServerFixture();
		using RawData = std::string;
		using TestMcuServer = mcu_server::McuServer<GpioId, RawData, RawData>;
		
		McuEngine *mcu_engine() const {
			return m_mcu_engine.get();
		}

		mcu_server::DataReceiver<RawData, RawData> *receiver() const {
			return m_receiver.get();
		}

		const mcu_server::Parser<engine::Data *(const RawData&)>& parser() const {
			return *m_parser;
		}

		const mcu_server::Serializer<RawData(const engine::Data&)>& serializer() const {
			return *m_serializer;
		}

		RawData head() const {
			return m_head;
		}

		RawData tail() const {
			return m_tail;
		}
	private:
		std::unique_ptr<McuEngine> m_mcu_engine;
		std::unique_ptr<mcu_server::DataReceiver<RawData, RawData>> m_receiver;
		std::unique_ptr<mcu_server::Parser<engine::Data *(const RawData&)>> m_parser;
		std::unique_ptr<mcu_server::Serializer<RawData(const engine::Data&)>> m_serializer;
		std::string m_head;
		std::string m_tail;
	};

	inline McuServerFixture::McuServerFixture(): McuTaskEngineFixture(), m_head("test_head"), m_tail("test_tail") {
		m_mcu_engine = std::unique_ptr<McuEngine>(
			new mcu_task_engine::McuTaskEngine<GpioId>(
				ctor_id_retriever(),
				failure_reporter(),
				gpio_id_retriever(),
				gpio_dir_retriever(),
				gpio_state_retriever(),
				sequence_retriever(),
				delay_retriever(),
				gpio_creator(),
				gpio_report_creator(),
				gpio_get_report_creator(),
				delay_ctor()
			)
		);
		m_receiver = std::unique_ptr<mcu_server::DataReceiver<RawData, RawData>>(
			new mcu_server_utl::CustomReceiver(m_head, m_tail)
		);

		m_parser = std::unique_ptr<mcu_server::Parser<engine::Data *(const RawData&)>>(
			new mcu_server_utl::JsonDataParser()
		);
		m_serializer = std::unique_ptr<mcu_server::Serializer<RawData(const engine::Data&)>>(
			new mcu_server_utl::JsonDataSerializer()
		);
	}
}

#endif