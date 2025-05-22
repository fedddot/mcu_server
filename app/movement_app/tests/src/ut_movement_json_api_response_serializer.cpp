#include <string>

#include "json/value.h"
#include "gtest/gtest.h"

#include "movement_json_api_response_serializer.hpp"
#include "movement_vendor_api_response.hpp"

using namespace ipc;
using namespace vendor;

TEST(ut_movement_json_api_response_serializer, ctor_dtor_sanity) {
	// WHEN
	MovementJsonApiResponseSerializer *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new MovementJsonApiResponseSerializer());
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_json_api_response_serializer, serialize_sanity) {
    // GIVEN
    const auto response_wo_message = MovementVendorApiResponse(
        MovementVendorApiResponse::Result::SUCCESS,
        std::nullopt
    );
    const auto response_w_message = MovementVendorApiResponse(
        MovementVendorApiResponse::Result::FAILURE,
        std::string("test message")
    );

    // WHEN
    const MovementJsonApiResponseSerializer instance;
    
    // THEN
    const auto json_wo_message = instance(response_wo_message);
    ASSERT_TRUE(json_wo_message.isMember("status"));
    ASSERT_EQ(json_wo_message["status"].asString(), "SUCCESS");
    ASSERT_FALSE(json_wo_message.isMember("message"));
    
    const auto json_w_message = instance(response_w_message);
    ASSERT_TRUE(json_w_message.isMember("status"));
    ASSERT_EQ(json_w_message["status"].asString(), "FAILURE");
    ASSERT_TRUE(json_w_message.isMember("message"));
    ASSERT_EQ(json_w_message["message"].asString(), "test message");
}