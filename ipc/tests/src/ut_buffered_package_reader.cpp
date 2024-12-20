#include "gtest/gtest.h"

#include "buffered_package_reader.hpp"

using namespace ipc;

TEST(ut_buffered_package_reader, ctor_dtor_sanity) {
    // GIVEN
    const BufferedPackageReader::Preamble preamble {'a', 'b', 'c'};

    // WHEN
    BufferedPackageReader *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = new BufferedPackageReader(preamble));
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}