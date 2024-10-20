#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"

#include "data.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "movement.hpp"
#include "movement_manager.hpp"
#include "object.hpp"
#include "server_types.hpp"
#include "string.hpp"
#include "test_movement.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;

TEST(ut_movement_manager, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, Movement> inventory;
	auto movement_ctor = [](const Data&) -> Movement * {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	auto movement_reader = [](const Movement&) -> Body {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	using MovementManagerUnqPtr = std::unique_ptr<MovementManager>;
	MovementManagerUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = MovementManagerUnqPtr(new MovementManager(&inventory, movement_ctor, movement_reader)));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_movement_manager, crud_methods_sanity) {
	// GIVEN
	Object config;
	config.add("param", Integer(114));
	Body create_body;
	create_body.add("id", String("test_id"));
	create_body.add("config", config);

	Body update_body;
	update_body.add("config", config);

	InMemoryInventory<ResourceId, Movement> inventory;
	
	// WHEN
	auto movement_ctor = [create_body, config](const Data& data) -> Movement * {
		if (Data::cast<String>(create_body.access("id")).get() != Data::cast<String>(Data::cast<Object>(data).access("id")).get()) {
			throw std::invalid_argument("invalid id received");
		}
		return new TestMovement(
			[config](const Data& data) {
				// THEN
				ASSERT_EQ(Data::cast<Integer>(config.access("param")).get(), Data::cast<Integer>(Data::cast<Object>(data).access("param")).get());
			}
		);
	};
	auto movement_reader = [](const Movement&) -> Body {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	MovementManager instance(&inventory, movement_ctor, movement_reader);
	ASSERT_NO_THROW(instance.create_resource(create_body));
	ASSERT_NO_THROW(instance.update_resource({Data::cast<String>(create_body.access("id")).get()}, update_body));
}