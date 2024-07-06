#ifndef MCU_SERVER_FIXTURE_HPP
#define MCU_SERVER_FIXTURE_HPP

#include "mcu_task_engine_fixture.hpp"

namespace mcu_server_uts {

	class McuServerFixture: public mcu_task_engine_uts::McuTaskEngineFixture {
	public:
		McuServerFixture();
	};

	inline McuServerFixture::McuServerFixture(): McuTaskEngineFixture() {

	}
}

#endif