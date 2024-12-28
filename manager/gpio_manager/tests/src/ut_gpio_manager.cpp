#include <memory>

#include "gtest/gtest.h"
#include <stdexcept>
#include <string>

#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "gpio_provider.hpp"
#include "gpo.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"

using namespace manager;
using namespace manager_uts;

using GpioId = std::string;

class TestGpioProvider: public GpioProvider<GpioId> {
public:
	TestGpioProvider() = default;
	TestGpioProvider(const TestGpioProvider&) = default;
	TestGpioProvider& operator=(const TestGpioProvider&) = default;

	Gpio *create_gpio(const GpioId& id, const Gpio::Direction& direction) const override {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
};

TEST(ut_gpio_manager, ctor_dtor_sanity) {
	// GIVEN
	TestGpioProvider provider;

	// WHEN
	GpioManager<GpioId> *instance_ptr(nullptr);
	
	// THEN
	ASSERT_NO_THROW(instance_ptr = new GpioManager<GpioId>(&provider));
	ASSERT_NO_THROW(delete instance_ptr);
}