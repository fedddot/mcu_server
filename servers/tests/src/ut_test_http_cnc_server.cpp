#include "gtest/gtest.h"

#include "gpio.hpp"
#include "json_data_serializer.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_fixture.hpp"
#include "server_types.hpp"

using namespace server;
using namespace server_utl;
using namespace cnc_server;

TEST_F(ServerFixture, gpio_sanity) {
    // GIVEN
    const auto test_gpio_id = ResourceId("test_gpio");
    const auto test_gpio_dir = manager::Gpio::Direction::OUT;
    const auto test_gpio_pin_number = 12;

    // WHEN
    const auto create_request = create_gpio_request(test_gpio_id, test_gpio_dir, test_gpio_pin_number);
    const auto delete_request = Request(Request::Method::DELETE, {"gpios", test_gpio_id}, Body());
    Response response(ResponseCode::UNSPECIFIED, Body());

    // THEN
    ASSERT_NO_THROW(response = run_request(create_request));
    const auto response_body = JsonDataSerializer()(response.body());
    ASSERT_EQ(ResponseCode::OK, response.code());
}
