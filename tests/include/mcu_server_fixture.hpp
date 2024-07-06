#ifndef MCU_SERVER_FIXTURE_HPP
#define MCU_SERVER_FIXTURE_HPP

#include <memory>

#include "data.hpp"
#include "engine.hpp"
#include "mcu_task_engine.hpp"
#include "mcu_task_engine_fixture.hpp"

namespace mcu_server_uts {

	class McuServerFixture: public mcu_task_engine_uts::McuTaskEngineFixture {
	public:
		using McuEngine = engine::Engine<engine::Data *(const engine::Data&)>;
		McuServerFixture();
		
		McuEngine *mcu_engine() const {
			return m_mcu_engine.get();
		}
	private:
		std::unique_ptr<McuEngine> m_mcu_engine;
	};

	inline McuServerFixture::McuServerFixture(): McuTaskEngineFixture() {
		m_mcu_engine = std::unique_ptr<McuEngine>(
			new mcu_task_engine::McuTaskEngine<GpioId>(
				ctor_id_retriever(),
				failure_reporter(),
				gpio_id_retriever(),
				gpio_dir_retriever(),
				gpio_state_retriever(),
				gpio_creator(),
				gpio_report_creator(),
				gpio_get_report_creator()
			)
		);
	}
}

#endif