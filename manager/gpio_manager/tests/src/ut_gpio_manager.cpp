#include <memory>

#include "gtest/gtest.h"

#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;

static Gpio *create_test_gpio(const Body& request_body) {
	const auto direction(static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(request_body.access("config")).access("dir")).get()));
	switch (direction) {
	case Gpio::Direction::IN:
		return new TestGpi();
	case Gpio::Direction::OUT:
		return new TestGpo();
	default:
		throw ServerException(ResponseCode::BAD_REQUEST, "unsupported direction received");
	}
}

TEST(ut_gpio_manager, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, Gpio> inventory;

	// WHEN
	using GpioManagerUnqPtr = std::unique_ptr<GpioManager>;
	GpioManagerUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = GpioManagerUnqPtr(new GpioManager(&inventory, create_test_gpio)));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_gpio_manager, crud_methods_sanity) {
	// GIVEN
	const auto dir(Gpio::Direction::OUT);
	const auto id(ResourceId("test_gpo"));
	const auto state(Gpio::State::HIGH);

	Object create_cfg;
	create_cfg.add("dir", Integer(static_cast<int>(dir)));
	
	Body create_body;
	create_body.add("id", String(id));
	create_body.add("config", create_cfg);

	// WHEN
	InMemoryInventory<ResourceId, Gpio> inventory;
	GpioManager instance(&inventory, create_test_gpio);

	// THEN
	ASSERT_NO_THROW(instance.create_resource(create_body));
	ASSERT_TRUE(inventory.contains(id));
	ASSERT_EQ(dir, inventory.access(id).direction());

	ASSERT_NO_THROW(instance.delete_resource({id}));
	ASSERT_FALSE(inventory.contains(id));
}