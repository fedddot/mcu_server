#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"

#include "data.hpp"
#include "movement.hpp"
#include "movement_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "server_types.hpp"
#include "test_movement.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;

TEST(ut_movement_manager, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, Movement> inventory;

	// WHEN
	std::unique_ptr<MovementManager> instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(
		instance_ptr = std::unique_ptr<MovementManager>(
			new MovementManager(
				&inventory,
				[](const Data& create_cfg)-> Movement * {
					throw std::runtime_error("not implemented");
				}
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_movement_manager, crud_methods_sanity) {
	// GIVEN
	const auto id(ResourceId("test_movement"));
	const auto movement_type(Movement::Type::LINEAR);

	Object create_cfg;
	create_cfg.add("type", Integer(static_cast<int>(movement_type)));

	Object update_cfg;
	update_cfg.add("param", Integer(10));

	auto movement_action = [update_cfg](const Data& movement_cfg) {
		ASSERT_EQ(Data::cast<Integer>(update_cfg.access("param")).get(), Data::cast<Integer>(Data::cast<Object>(movement_cfg).access("param")).get());
	};

	auto movement_ctor = [movement_action](const Data& create_cfg) {
		auto type(static_cast<Movement::Type>(Data::cast<Integer>(Data::cast<Object>(create_cfg).access("type")).get()));
		return new TestMovement(movement_action, type);
	};

	// WHEN
	InMemoryInventory<ResourceId, Movement> inventory;
	MovementManager instance(&inventory, movement_ctor);
	Object read_data;

	// THEN
	// create
	ASSERT_NO_THROW(instance.create_resource(id, create_cfg));
	ASSERT_TRUE(inventory.contains(id));
	ASSERT_EQ(movement_type, inventory.access(id).type());

	// read
	ASSERT_NO_THROW(read_data = instance.read_resource({id}));
	ASSERT_TRUE(read_data.contains("type"));
	ASSERT_EQ(movement_type, static_cast<Movement::Type>(Data::cast<Integer>(read_data.access("type")).get()));

	// update
	ASSERT_NO_THROW(instance.update_resource({id}, update_cfg));

	// delete
	ASSERT_NO_THROW(instance.delete_resource({id}));
	ASSERT_FALSE(inventory.contains(id));
}