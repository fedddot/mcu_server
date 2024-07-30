#include <string>

#include "gtest/gtest.h"

#include "tcp_ipc_server.hpp"

using namespace mcu_platform;
using namespace mcu_platform_utl;

TEST(ut_tcp_ipc_server, ctor_dtor) {
	// WHEN
	TcpIpcServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TcpIpcServer("127.0.0.1", 1212, "test_header", "test_tail")
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}