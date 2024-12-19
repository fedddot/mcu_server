#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "array.hpp"
#include "cnc_server.hpp"
#include "data.hpp"
#include "double.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "movement.hpp"
#include "object.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "string.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"
#include "test_ipc_connection.hpp"

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
            [](const double) {
                throw std::runtime_error("NOT IMPLEMENTED");
            }
        )
    );
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(instance_ptr = nullptr);
}

static void create_gpio(TestConnection *connection, const ResourceId& gpio_id, const Gpio::Direction& dir) {
    // GIVEN
    Object test_gpio_cfg;
    test_gpio_cfg.add("direction", Integer(static_cast<int>(dir)));
    Object create_gpio_cfg;
    create_gpio_cfg.add("id", String(gpio_id));
    create_gpio_cfg.add("config", test_gpio_cfg);

    // WHEN
    Response result(ResponseCode::UNSPECIFIED, Object());

    // THEN
    ASSERT_NO_THROW(
        connection->publish_request(
            Request(
                Request::Method::CREATE,
                {"gpios"},
                create_gpio_cfg
            )
        )
    );
}

static void create_stepper(TestConnection *connection, const ResourceId& stepper_id) {
    Object control_outputs;
    for (const auto& gpo_tag: {"ena", "enb"}) {
        const auto gpo_id = stepper_id + "_" + gpo_tag;
        create_gpio(connection, gpo_id, Gpo::Direction::OUT);
        control_outputs.add(gpo_tag, String(gpo_id));
    }
    Object direction_outputs;
    for (const auto& gpo_tag: {"a_top", "a_btm", "b_top", "b_btm"}) {
        const auto gpo_id = stepper_id + "_" + gpo_tag;
        create_gpio(connection, gpo_id, Gpo::Direction::OUT);
        direction_outputs.add(gpo_tag, String(gpo_id));
    }
    Object state0;
	state0.add("a_top", Integer(static_cast<int>(Gpo::State::HIGH)));
	state0.add("a_btm", Integer(static_cast<int>(Gpo::State::LOW)));
	state0.add("b_top", Integer(static_cast<int>(Gpo::State::LOW)));
	state0.add("b_btm", Integer(static_cast<int>(Gpo::State::LOW)));
	Object state1;
	state1.add("a_top", Integer(static_cast<int>(Gpo::State::LOW)));
	state1.add("a_btm", Integer(static_cast<int>(Gpo::State::LOW)));
	state1.add("b_top", Integer(static_cast<int>(Gpo::State::HIGH)));
	state1.add("b_btm", Integer(static_cast<int>(Gpo::State::LOW)));
	Array states;
	states.push_back(state0);
	states.push_back(state1);

    Object create_stepper_cfg;
    create_stepper_cfg.add("control_outputs", control_outputs);
    create_stepper_cfg.add("direction_outputs", direction_outputs);
    create_stepper_cfg.add("states", states);
    
    Body create_stepper_body;
    create_stepper_body.add("id", String(stepper_id));
    create_stepper_body.add("config", create_stepper_cfg);
    Request create_gpio_request(Request::Method::CREATE, Path {"steppers"}, create_stepper_body);
    connection->publish_request(create_gpio_request);
}

TEST(ut_cnc_server, run_movement_sanity) {
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
    const unsigned int steps_per_length(100UL);
    auto delay_function = [](const double& delay) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned int>(1000 * delay)));
    };
    TestConnection connection(
        [](const Response& response)-> void {
            std::cout << "cnc_server sends response, code = " << std::to_string(static_cast<int>(response.code())) << std::endl;
            ASSERT_EQ(ResponseCode::OK, response.code());
        }
    );
    Object motors_assignment;
    motors_assignment.add("x", String("motor_x"));
    motors_assignment.add("y", String("motor_y"));
    motors_assignment.add("z", String("motor_z"));

    Object linear_movement_cfg;
    linear_movement_cfg.add("steps_per_length", Integer(100));
    linear_movement_cfg.add("steppers", motors_assignment);
    linear_movement_cfg.add("type", Integer(static_cast<int>(Movement::Type::LINEAR)));

    Object linear_movement_create_cfg;
    linear_movement_create_cfg.add("id", String("linear_movement"));
    linear_movement_create_cfg.add("config", linear_movement_cfg);


    Object movement_target;
    movement_target.add("x", Double(0.1));
    movement_target.add("y", Double(0.1));
    movement_target.add("z", Double(3.1));

    const Double feed(10.1);

    Object linear_movement_data;
    linear_movement_data.add("target", movement_target);
    linear_movement_data.add("feed", feed);

    // WHEN
    TestServer instance(&connection, test_id, gpio_ctor, delay_function);

    // THEN
    ASSERT_FALSE(instance.is_running());
    ASSERT_NO_THROW(instance.run());
    ASSERT_TRUE(instance.is_running());
    motors_assignment.for_each(
        [&connection](const std::string&, const Data& motor_id) {
            create_stepper(&connection, static_cast<ResourceId>(Data::cast<String>(motor_id).get()));
        }
    );
    ASSERT_NO_THROW(
        connection.publish_request(
            Request(Request::Method::CREATE, {"movements"}, linear_movement_create_cfg)
        )
    );
    ASSERT_NO_THROW(
        connection.publish_request(
            Request(Request::Method::UPDATE, {"movements", Data::cast<String>(linear_movement_create_cfg.access("id")).get()}, linear_movement_data)
        )
    );
    ASSERT_NO_THROW(
        connection.publish_request(
            Request(Request::Method::DELETE, {"movements", Data::cast<String>(linear_movement_create_cfg.access("id")).get()}, Body())
        )
    );
    ASSERT_NO_THROW(instance.stop());
    ASSERT_FALSE(instance.is_running());
}