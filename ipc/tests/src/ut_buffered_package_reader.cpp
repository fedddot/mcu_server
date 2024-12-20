#include "gtest/gtest.h"
#include <string>

#include "buffered_package_reader.hpp"
#include "ipc_types.hpp"

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

static RawData serialize_size(const PackageSize& size) {
    enum: int {BITS_IN_BYTE = 8};
    RawData buff;
    buff.reserve(sizeof(PackageSize));
    for (auto byte_number = 0; byte_number < sizeof(PackageSize); ++byte_number) {
        const auto ch = static_cast<char>((size >> (byte_number * BITS_IN_BYTE)) & 0xFF);
        buff.push_back(ch);
    }
    return buff;
}

TEST(ut_buffered_package_reader, readable_read_feed_sanity) {
    // GIVEN
    const std::string preamble_str("test_preamble");
    const BufferedPackageReader::Preamble preamble(preamble_str.begin(), preamble_str.end());
    const std::string expected_package_str("lady gaga");
    const Package expected_package(expected_package_str.begin(), expected_package_str.end());

    // WHEN
    BufferedPackageReader instance(preamble);
    Package result;

    // THEN
    ASSERT_FALSE(instance.readable());
    for (const auto ch: preamble) {
        instance.feed(ch);
    }
    ASSERT_FALSE(instance.readable());
    const auto size_serialized = serialize_size(expected_package.size());
    for (const auto ch: size_serialized) {
        instance.feed(ch);
    }
    ASSERT_FALSE(instance.readable());
    for (const auto ch: expected_package) {
        instance.feed(ch);
    }
    ASSERT_TRUE(instance.readable());
    ASSERT_NO_THROW(result = instance.read());
    ASSERT_EQ(expected_package, result);
    ASSERT_FALSE(instance.readable());
}