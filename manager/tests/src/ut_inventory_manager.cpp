#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "in_memory_inventory.hpp"
#include "inventory_manager.hpp"
#include "object.hpp"
#include "server_types.hpp"

using namespace manager;
using namespace server;

TEST(ut_inventory_manager, ctor_dtor_sanity) {
    // GIVEN
    auto string_ctor = [](const Data& create_cfg) -> std::string * {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    auto string_reader = [](const std::string& item) -> Object {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    auto string_writer = [](std::string *item, const Data& update_cfg) {
        throw std::runtime_error("NOT IMPLEMENTED");
    };

    // WHEN
    InMemoryInventory<ResourceId, std::string> inventory;
    InventoryManager<std::string> *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new InventoryManager<std::string>(
            &inventory,
            string_ctor,
            string_reader,
            string_writer
        )
    );
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}