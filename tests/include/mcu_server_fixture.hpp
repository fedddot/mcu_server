#ifndef MCU_SERVER_FIXTURE_HPP
#define MCU_SERVER_FIXTURE_HPP

#include "mcu_factory_fixture.hpp"

namespace mcu_server_uts {
	class McuServerFixture: public mcu_factory_uts::McuFactoryFixture {
	public:
		McuServerFixture(const std::string& msg_head = "test_msg_head", const std::string& msg_tail = "test_msg_tail", const std::size_t& max_buffer_size = 1000UL): mcu_factory_uts::McuFactoryFixture(msg_head, msg_tail, max_buffer_size) {
		
		}
		McuServerFixture(const McuServerFixture& other) = delete;
		McuServerFixture& operator=(const McuServerFixture& other) = delete;
	
	private:
		
	};
}

#endif