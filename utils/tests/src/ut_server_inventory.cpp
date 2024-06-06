#include "gtest/gtest.h"
#include <string>
#include "server_inventory.hpp"

using namespace server_utl;
// using namespace engine;

TEST(ut_server_inventory, ctor_dtor_sanity) {
	// WHEN
	ServerInventory<int, std::string> *instance_ptr;

	// THEN
	ASSERT_NO_THROW(
		(instance_ptr = new ServerInventory<int, std::string>())
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	// CLEANUP
	instance_ptr = nullptr;
}