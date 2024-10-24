#include <memory>

#include "gtest/gtest.h"

#include "data.hpp"
#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "gpo.hpp"
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

static Gpio *create_test_gpio(const Data& create_cfg) {
	const auto direction(static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(create_cfg).access("dir")).get()));
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
	
	Object update_cfg;
	update_cfg.add("state", Integer(static_cast<int>(state)));

	// WHEN
	InMemoryInventory<ResourceId, Gpio> inventory;
	GpioManager instance(&inventory, create_test_gpio);
	Body response_body;

	// THEN
	// create
	ASSERT_NO_THROW(instance.create_resource(id, create_cfg));
	ASSERT_TRUE(inventory.contains(id));
	ASSERT_EQ(dir, inventory.access(id).direction());

	// read
	ASSERT_NO_THROW(response_body = instance.read_resource({id}));
	ASSERT_TRUE(response_body.contains("direction"));
	ASSERT_EQ(dir, static_cast<Gpio::Direction>(Data::cast<Integer>(response_body.access("direction")).get()));
	ASSERT_EQ(Gpio::State::LOW, static_cast<Gpio::State>(Data::cast<Integer>(response_body.access("state")).get()));

	// update
	ASSERT_NO_THROW(instance.update_resource({id}, update_cfg));
	ASSERT_EQ(state, Gpio::cast<Gpo>(inventory.access(id)).state());	

	// delete
	ASSERT_NO_THROW(instance.delete_resource({id}));
	ASSERT_FALSE(inventory.contains(id));
}