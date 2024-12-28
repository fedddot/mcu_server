#include <memory>

#include "gtest/gtest.h"

#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "gpo.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"

using namespace manager;
using namespace manager_uts;

TEST(ut_gpio_manager, ctor_dtor_sanity) {
	// GIVEN
	
	// WHEN
	GpioManager *instance_ptr(nullptr);
	
	// THEN
}