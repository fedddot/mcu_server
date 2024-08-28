#ifndef FACTORY_FIXTURE_HPP
#define FACTORY_FIXTURE_HPP

#include "gtest/gtest.h"

namespace mcu_factory_uts {
	class FactoryFixture: public testing::Test {
	public:
		FactoryFixture() = default;
		FactoryFixture(const FactoryFixture&) = delete;
		FactoryFixture& operator=(const FactoryFixture&) = delete;
	};
}

#endif