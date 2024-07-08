#include "gtest/gtest.h"

#include "custom_sender.hpp"
#include "mcu_server.hpp"
#include "mcu_server_fixture.hpp"

using namespace mcu_server;
using namespace mcu_server_uts;
using namespace mcu_server_utl;

using TestMcuServer = McuServer<McuServerFixture::GpioId, McuServerFixture::RawData, McuServerFixture::RawData>;

TEST_F(McuServerFixture, ctor_dtor_sanity) {
	// GIVEN
	CustomSender<RawData> sender(
		[](const RawData&) {

		}
	);

	// WHEN
	TestMcuServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestMcuServer(
				mcu_engine(),
				&sender,
				receiver(),
				parser(),
				serializer()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}