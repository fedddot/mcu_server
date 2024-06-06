#include <stdexcept>

#include "gtest/gtest.h"

#include "server_parser.hpp"

using namespace server_utl;

TEST(ut_server_parser, ctor_dtor_sanity) {
	// WHEN
	server_utl::ServerParser<int(const std::string&)> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new ServerParser<int(const std::string&)>(
				[](const std::string&)-> int {
					throw std::runtime_error("not implemented");
				}
			)
		)
	);
	ASSERT_NO_THROW(delete instance_ptr);
}