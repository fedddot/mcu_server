#include <string>

#include "gtest/gtest.h"

#include "movement_host_builder.hpp"

using namespace ipc;
// using namespace manager;
using namespace host;

using AxesConfig = std::string;

TEST(ut_movement_host_builder, ctor_dtor_sanity) {
	// WHEN
	MovementHostBuilder<AxesConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new MovementHostBuilder<AxesConfig>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_host_builder, build_sanity) {
	// WHEN
	MovementHostBuilder<AxesConfig> instance;

	// THEN
	ASSERT_NO_THROW(
		{
			const auto host = instance.build();
		}
	);
}