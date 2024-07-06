#include "gtest/gtest.h"

#include "mcu_server.hpp"
#include "mcu_server_fixture.hpp"

using namespace mcu_server;
using namespace mcu_server_uts;

using GpioId = mcu_server_uts::McuServerFixture::GpioId;

TEST_F(McuServerFixture, ctor_dtor_sanity) {
	// WHEN
	McuServer<GpioId> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new McuServer<GpioId>()
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}