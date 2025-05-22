#include <string>

#include "json/value.h"
#include "gtest/gtest.h"

#include "axes_controller_config_request.hpp"
#include "linear_movement_request.hpp"
#include "movement_json_api_request_parser.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"
#include "movement_vendor_api_request.hpp"

using namespace ipc;
using namespace vendor;
using namespace manager;

using AxesConfig = std::string;
static Json::Value vector_to_json_val(const Vector<double>& vec);

TEST(ut_movement_json_api_request_parser, ctor_dtor_sanity) {
	// WHEN
	MovementJsonApiRequestParser<AxesConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new MovementJsonApiRequestParser<AxesConfig>(
				[](const Json::Value&) -> AxesConfig {
					return "axes_config";
				}
			)
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_json_api_request_parser, parse_config_request) {
	// GIVEN
	const auto expected_request_type = MovementVendorApiRequest::RequestType::CONFIG;
	const auto expected_axes_config = "test_axes_config";
	Json::Value json_value;
	json_value["request_type"] = "CONFIG";
	json_value["axes_config"] = expected_axes_config;
	
	// WHEN
	MovementJsonApiRequestParser<AxesConfig> instance(
		[](const Json::Value& json_request) -> AxesConfig {
			return json_request["axes_config"].asString();
		}
	);
	
	// THEN
	ASSERT_NO_THROW({
		const auto request = instance(json_value);
		ASSERT_EQ(expected_request_type, request.get().type());
		const auto& casted_request = dynamic_cast<const AxesControllerConfigApiRequest<AxesConfig>&>(request.get());
		ASSERT_EQ(expected_axes_config, casted_request.axes_cfg());
	});
}

TEST(ut_movement_json_api_request_parser, parse_linear_movement_request) {
    // GIVEN
    const auto expected_request_type = MovementVendorApiRequest::RequestType::LINEAR_MOVEMENT;
    const auto destination = Vector<double>(0.1, 0.2, 0.3);
    const auto speed = 0.5;

    Json::Value json_value;
    json_value["request_type"] = "LINEAR_MOVEMENT";
    json_value["destination"] = vector_to_json_val(destination);
    json_value["speed"] = speed;
    
    // WHEN
    MovementJsonApiRequestParser<AxesConfig> instance(
        [](const Json::Value& json_request) -> AxesConfig {
            return json_request["axes_config"].asString();
        }
    );
    
    // THEN
    ASSERT_NO_THROW({
        const auto request = instance(json_value);
        ASSERT_EQ(expected_request_type, request.get().type());
        const auto& casted_request = dynamic_cast<const LinearMovementRequest&>(request.get());
    });
}

TEST(ut_movement_json_api_request_parser, parse_rotational_movement_request) {
    // GIVEN
    const auto expected_request_type = MovementVendorApiRequest::RequestType::ROTATIONAL_MOVEMENT;
    const auto destination = Vector<double>(1.1, 2.2, 3.3);
    const auto rotation_center = Vector<double>(4.4, 5.5, 6.6);
    const auto angle = 45.0;
    const auto speed = 1.23;

    Json::Value json_value;
    json_value["request_type"] = "ROTATIONAL_MOVEMENT";
    json_value["destination"] = vector_to_json_val(destination);
    json_value["rotation_center"] = vector_to_json_val(rotation_center);
    json_value["angle"] = angle;
    json_value["speed"] = speed;

    // WHEN
    MovementJsonApiRequestParser<AxesConfig> instance(
        [](const Json::Value& json_request) -> AxesConfig {
            return json_request["axes_config"].asString();
        }
    );

    // THEN
    ASSERT_NO_THROW({
        const auto request = instance(json_value);
        ASSERT_EQ(expected_request_type, request.get().type());
        const auto& casted_request = dynamic_cast<const RotationMovementRequest&>(request.get());
    });
}

inline static Json::Value vector_to_json_val(const Vector<double>& vec) {
    Json::Value json;
    json["x"] = vec.get(Axis::X);
    json["y"] = vec.get(Axis::Y);
    json["z"] = vec.get(Axis::Z);
    return json;
}