#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"
#include <thread>

#include "data.hpp"
#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "integer.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "test_ipc_connection.hpp"
#include "test_resource.hpp"
#include "vendor.hpp"

using namespace server;
using namespace vendor;
using namespace manager;

TEST(ut_server, ctor_cctor_dtor_sanity) {
    // GIVEN
    const server_uts::TestResource test_vendor(
        [](const Request&)-> Response {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );
    server_uts::TestIpcConnection connection(
        [](const Response&)-> void {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    std::unique_ptr<Server> instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = std::unique_ptr<Server>(new Server(&connection, test_vendor)));
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(instance_ptr = nullptr);
}


TEST(ut_server, run_is_running_stop_sanity) {
    // GIVEN
    Vendor test_vendor;
    test_vendor.register_resource(
        "gpios",
        GpioManager(
            [](const Data&)-> Gpio * {
                throw std::runtime_error("NOT IMPLEMENTED");
            }
        )
    );
    server_uts::TestIpcConnection connection(
        [](const Response&)-> void {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    Server instance(&connection, test_vendor);
    Request::Body create_gpio_body;
    create_gpio_body.add("id", Integer(1));
    create_gpio_body.add("dir", Integer(static_cast<int>(Gpio::Direction::OUT)));
    Request create_gpio_request(Request::Method::CREATE, Request::Path {"gpios"}, create_gpio_body);

    // THEN
    ASSERT_NO_THROW(connection.publish_request(create_gpio_request));
}