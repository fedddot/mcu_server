#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "gpio.hpp"
#include "gpio_create_request.hpp"
#include "gpio_delete_request.hpp"
#include "gpio_manager.hpp"
#include "gpio_provider.hpp"
#include "gpio_read_request.hpp"
#include "gpio_response.hpp"
#include "gpio_state_response.hpp"
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
		switch (direction) {
		case Gpio::Direction::IN:
			return new TestGpi();
		case Gpio::Direction::OUT:
			return new TestGpo();
		default:
			throw std::invalid_argument("unsupported direction");
		}
	}
};

TEST(ut_gpio_manager, ctor_dtor_sanity) {
	// GIVEN
	TestGpioProvider provider;
	GpioCreateRequest<GpioId> create_request("test_gpio", Gpio::Direction::OUT);
	GpioReadRequest<GpioId> get_request("test_gpio");
	GpioDeleteRequest<GpioId> delete_request("test_gpio");
	
	// WHEN
	GpioManager<GpioId> *instance_ptr(nullptr);
	GpioResponseWrapper result;

	// THEN
	ASSERT_NO_THROW(instance_ptr = new GpioManager<GpioId>(&provider));
	
	ASSERT_NO_THROW(result = instance_ptr->run(create_request));
	ASSERT_EQ(GpioResponse::Type::REGULAR, result->type());
	ASSERT_EQ(GpioResponse::Result::SUCCESS, result->result());
	
	ASSERT_NO_THROW(result = instance_ptr->run(get_request));
	ASSERT_EQ(GpioResponse::Type::STATE_RESPONSE, result->type());
	ASSERT_EQ(GpioResponse::Result::SUCCESS, result->result());
	ASSERT_EQ(Gpio::State::LOW, dynamic_cast<const GpioStateResponse&>(*result).state());
	
	ASSERT_NO_THROW(result = instance_ptr->run(delete_request));
	ASSERT_EQ(GpioResponse::Type::REGULAR, result->type());
	ASSERT_EQ(GpioResponse::Result::SUCCESS, result->result());
	
	ASSERT_NO_THROW(delete instance_ptr);
}