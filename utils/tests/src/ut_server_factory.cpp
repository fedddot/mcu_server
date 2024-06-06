#include "gtest/gtest.h"
#include <stdexcept>
#include <string>
#include "server_factory.hpp"
#include "server_parser.hpp"

using namespace server_utl;
// using namespace engine;

TEST(ut_server_factory, ctor_dtor_sanity) {
	// GIVEN
	ServerParser<int(const int&)> test_parser(
		[](const int& data)-> int {
			throw std::runtime_error("not needed");
		}
	);
	// WHEN
	ServerFactory<int, std::string, int> *instance_ptr;

	// THEN
	ASSERT_NO_THROW(
		(instance_ptr = new ServerFactory<int, std::string, int>(test_parser))
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	// CLEANUP
	instance_ptr = nullptr;
}