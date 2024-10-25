#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "cnc_server.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "linear_movement.hpp"
#include "object.hpp"
#include "response.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"
#include "test_ipc_connection.hpp"
#include "test_stepper_motor.hpp"

using namespace server;
using namespace cnc_server;
using namespace manager;
using namespace manager_uts;
using namespace ipc_uts;
using namespace vendor;

using TestServer = CncServer<std::string>;
using TestConnection = TestIpcConnection<std::string>;

TEST(ut_cnc_server, ctor_dtor_sanity) {
    // GIVEN
    const std::string test_id("test_cnc_server");
    TestConnection connection(
        [](const Response&)-> void {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    TestServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new TestServer(
            &connection,
            test_id,
            [](const Data&)-> Gpio * {
                throw std::runtime_error("NOT IMPLEMENTED");
            },
            [](const Data&)-> StepperMotor * {
                throw std::runtime_error("NOT IMPLEMENTED");
            },
            [](const LinearMovement::TimeUnit&) {
                throw std::runtime_error("NOT IMPLEMENTED");
            }
        )
    );
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_cnc_server, run_is_running_stop_sanity) {
    // GIVEN
    const std::string test_id("test_cnc_server");
    auto gpio_ctor = [](const Data& data)-> Gpio * {
        auto dir = static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("direction")).get());
        switch (dir) {
        case Gpio::Direction::IN:
            return new manager_uts::TestGpi();
        case Gpio::Direction::OUT:
            return new TestGpo();
        default:
            throw std::invalid_argument("invalid dir");
        }
    };
	auto steps_action_called(false);
	auto step_action = [&steps_action_called](const StepperMotor::Direction& dir) {
		std::cout << "a step in direction " << std::to_string(static_cast<int>(dir)) << std::endl;
		steps_action_called = true;
	};
    auto stepper_ctor = [step_action](const Data& data)-> StepperMotor * {
        return new TestStepperMotor(step_action);
    };
    const unsigned int time_multiplier(1000UL); // ms
    auto delay_function = [](const LinearMovement::TimeUnit& delay) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    };
    TestConnection connection(
        [](const Response& response)-> void {
            std::cout << "cnc_server sends response, code = " << std::to_string(static_cast<int>(response.code())) << std::endl;
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
    TestServer instance(&connection, test_id, gpio_ctor, stepper_ctor, delay_function);

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