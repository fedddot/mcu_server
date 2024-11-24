#include <memory>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "clonable_manager_wrapper.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "in_memory_inventory.hpp"
#include "object.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"
#include "integer.hpp"
#include "resources_vendor.hpp"
#include "response.hpp"
#include "server.hpp"
#include "server_types.hpp"
#include "test_gpi.hpp"
#include "test_ipc_connection.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;
using namespace ipc_uts;
using namespace vendor;

TEST(ut_custom_server, ctor_dtor_sanity) {
    // GIVEN
    const std::string test_id("test_server");
    vendor::ResourcesVendor vendor;
    TestConnection connection(
        [](const Response&)-> void {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    std::unique_ptr<TestServer> instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = std::unique_ptr<TestServer>(new TestServer(&connection, test_id, &vendor)));
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_custom_server, run_is_running_stop_sanity) {
    // GIVEN
    const std::string test_id("test_server");
    InMemoryInventory<ResourceId, Gpio> gpio_inventory;

    ResourcesVendor test_vendor;
    test_vendor.add_manager(
        "gpios",
        ClonableManagerWrapper(
            new GpioManager(
                &gpio_inventory,
                [](const Data& data)-> Gpio * {
                    auto dir = static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("dir")).get());
                    switch (dir) {
                    case Gpio::Direction::IN:
                        return new manager_uts::TestGpi();
                    case Gpio::Direction::OUT:
                        return new TestGpo();
                    default:
                        throw std::invalid_argument("invalid dir");
                    }
                }
            )
        )
    );
    TestConnection connection(
        [](const Response& response)-> void {
            std::cout << "server sends response, code = " << std::to_string(static_cast<int>(response.code())) << std::endl;
            ASSERT_EQ(ResponseCode::OK, response.code());
        }
    );

    Object create_gpio_cfg;
    Body create_gpio_body;
    create_gpio_cfg.add("dir", Integer(static_cast<int>(Gpio::Direction::OUT)));
    create_gpio_body.add("id", String("1"));
    create_gpio_body.add("config", create_gpio_cfg);
    Request create_gpio_request(Request::Method::CREATE, Path {"gpios"}, create_gpio_body);

    Body update_body;
    update_body.add("state", Integer(static_cast<int>(Gpio::State::HIGH)));
    Request update_gpio_request(Request::Method::UPDATE, Path {"gpios", "1"}, update_body);

    Request read_gpio_request(Request::Method::READ, Path {"gpios", "1"}, Body());

    Request delete_gpio_request(Request::Method::DELETE, Path {"gpios", "1"}, Body());

    // WHEN
    TestServer instance(&connection, test_id, &test_vendor);

    // THEN
    ASSERT_FALSE(instance.is_running());
    ASSERT_NO_THROW(instance.run());
    ASSERT_TRUE(instance.is_running());
    ASSERT_NO_THROW(connection.publish_request(create_gpio_request));
    ASSERT_NO_THROW(connection.publish_request(update_gpio_request));
    ASSERT_NO_THROW(connection.publish_request(read_gpio_request));
	ASSERT_NO_THROW(connection.publish_request(delete_gpio_request));
    ASSERT_NO_THROW(instance.stop());
    ASSERT_FALSE(instance.is_running());
}