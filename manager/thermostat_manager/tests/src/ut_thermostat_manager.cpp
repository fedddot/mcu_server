#include <stdexcept>
#include <thread>

#include "gtest/gtest.h"
#include "thermostat_manager.hpp"

using namespace manager;

TEST(ut_thermostat_manager, ctor_dtor_sanity) {
	// WHEN
	ThermostatManager *instance(nullptr);

	// THEN
	// ASSERT_NO_THROW(instance = new ThermostatManager(create_axes_ctrlr));
	ASSERT_NE(instance, nullptr);
	ASSERT_NO_THROW(delete instance);
}