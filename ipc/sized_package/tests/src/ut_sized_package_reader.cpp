#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "sized_package_reader.hpp"

using namespace ipc;

TEST(ut_sized_package_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto preamble_str = std::string("test_preamble");
	const auto preamble = std::vector<char>(preamble_str.begin(), preamble_str.end());

	// WHEN
	SizedPackageReader *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new SizedPackageReader(preamble));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}