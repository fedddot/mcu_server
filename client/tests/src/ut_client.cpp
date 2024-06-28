#include <string>

#include "gtest/gtest.h"

#include "client.hpp"
#include "functional_connection.hpp"

using namespace client;
using namespace mcu_control_utl;

using RawData = std::string;
using TestClient = Client<RawData>;



TEST(ut_client, ctor_dtor_sanity) {
	// GIVEN
	FunctionalConnection<RawData> test_connection(
		[](const RawData& data) {
			(void)(data);
		},
		[]() {
			return false;
		},
		[]() {
			return RawData("");
		} 
	);
	
	// WHEN
	TestClient *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestClient(&test_connection)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;	
}

TEST(ut_client, run_sanity_negative) {
	// GIVEN
	const

}