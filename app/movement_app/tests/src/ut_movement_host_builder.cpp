#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "movement_host_builder.hpp"

using namespace ipc;
// using namespace manager;
using namespace host;

using AxesConfig = std::string;
using RawData = std::string;

TEST(ut_movement_host_builder, ctor_dtor_sanity) {
	// WHEN
	MovementHostBuilder<AxesConfig, RawData> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new MovementHostBuilder<AxesConfig, RawData>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}