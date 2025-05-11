#include <string>

#include "gtest/gtest.h"
#include "json/value.h"
#include "movement_host_builder.hpp"

using namespace ipc;
using namespace manager;
using namespace host;

using AxisControllerConfig = std::string;

static Json::Value cfg2json(const AxisControllerConfig& cfg);
static AxisControllerConfig json2cfg(const Json::Value& cfg);

TEST(ut_movement_host_builder, ctor_dtor_sanity) {
	// WHEN
	MovementHostBuilder<AxisControllerConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new MovementHostBuilder<AxisControllerConfig>());
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_host_builder, build_sanity) {
	// WHEN
	MovementHostBuilder<AxisControllerConfig> instance;

	// THEN
	ASSERT_NO_THROW(const auto host = instance.build());
}