#include <iostream>
#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"
#include <string>

#include "data.hpp"
#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "test_ipc_connection.hpp"
#include "test_resource.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"
#include "vendor.hpp"

using namespace server;
using namespace server_uts;
using namespace vendor;
using namespace manager;
using namespace manager_uts;

using TestServer = Server<std::string>;
using TestConnection = TestIpcConnection<std::string>;

TEST(ut_server, ctor_dtor_sanity) {
    // GIVEN
    const std::string test_id("test_server");
    const TestResource test_vendor(
        [](const Request&)-> Response {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );
    TestConnection connection(
        [](const Response&)-> void {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    std::unique_ptr<TestServer> instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = std::unique_ptr<TestServer>(new TestServer(&connection, test_id, test_vendor)));
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_server, run_is_running_stop_sanity) {
    // GIVEN
    const std::string test_id("test_server");
    Vendor test_vendor;
    test_vendor.register_resource(
        "gpios",
        GpioManager(
            [](const Data& data)-> Gpio * {
                auto dir = static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("dir")).get());
                switch (dir) {
                case Gpio::Direction::IN:
                    return new TestGpi();
                case Gpio::Direction::OUT:
                    return new TestGpo();
                default:
                    throw std::invalid_argument("invalid dir");
                }
            }
        )
    );
    TestConnection connection(
        [](const Response& response)-> void {
            std::cout << "server sends response, code = " << std::to_string(static_cast<int>(response.code())) << std::endl;
            ASSERT_EQ(Response::ResponseCode::OK, response.code());
        }
    );

    Request::Body create_gpio_body;
    create_gpio_body.add("id", String("1"));
    create_gpio_body.add("dir", Integer(static_cast<int>(Gpio::Direction::OUT)));
    Request create_gpio_request(Request::Method::CREATE, Request::Path {"gpios"}, create_gpio_body);

    Request::Body update_gpio_body;
    update_gpio_body.add("state", Integer(static_cast<int>(Gpio::State::HIGH)));
    Request update_gpio_request(Request::Method::UPDATE, Request::Path {"gpios", "1"}, update_gpio_body);

    Request read_gpio_request(Request::Method::READ, Request::Path {"gpios", "1"}, Request::Body());

    Request delete_gpio_request(Request::Method::DELETE, Request::Path {"gpios", "1"}, Request::Body());

    // WHEN
    TestServer instance(&connection, test_id, test_vendor);

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