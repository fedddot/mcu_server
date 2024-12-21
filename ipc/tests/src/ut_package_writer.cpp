#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "custom_data_writer.hpp"
#include "package_writer.hpp"
#include "ipc_types.hpp"

using namespace ipc;

TEST(ut_package_writer, ctor_dtor_sanity) {
    // GIVEN
    const PackageWriter::Preamble preamble {'a', 'b', 'c'};

    // WHEN
    CustomDataWriter<RawData> raw_data_writer(
        [](const RawData& data) {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );
    PackageWriter *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = new PackageWriter(preamble, &raw_data_writer));
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}

static PackageSize parse_size(const RawData& raw_data) {
    enum: int {BITS_IN_BYTE = 8};
    if (sizeof(PackageSize) != raw_data.size()) {
        throw std::invalid_argument("invalid raw data received");
    }
    PackageSize result(0);
    for (auto byte_number = 0; byte_number < sizeof(PackageSize); ++byte_number) {
        const auto chunk = raw_data[sizeof(PackageSize) - 1 - byte_number];
        result <<= BITS_IN_BYTE;
        result |= chunk;
    }
    return result;
}

TEST(ut_package_writer, readable_read_feed_sanity) {
    // GIVEN
    const std::string preamble_str("test_preamble");
    const PackageWriter::Preamble preamble(preamble_str.begin(), preamble_str.end());
    const std::string test_package_str("lady gaga");
    const Package test_package(test_package_str.begin(), test_package_str.end());

    // WHEN
    CustomDataWriter<RawData> raw_data_writer(
        [preamble, test_package](const RawData& data) {
            const auto received_preamble = PackageWriter::Preamble(
                data.begin(),
                data.begin() + preamble.size()
            );
            ASSERT_EQ(preamble, received_preamble);
            const auto received_package_size_buffer = RawData(
                data.begin() + preamble.size(),
                data.begin() + preamble.size() + sizeof(PackageSize)
            );
            const auto received_package_size = parse_size(received_package_size_buffer);
            ASSERT_EQ(test_package.size(), received_package_size);
            const auto received_data = Package(
                data.begin() + preamble.size() + sizeof(PackageSize),
                data.begin() + preamble.size() + sizeof(PackageSize) + received_package_size
            );
            ASSERT_EQ(test_package, received_data);
        }
    );
    PackageWriter instance(preamble, &raw_data_writer);

    // THEN
    ASSERT_NO_THROW(instance.write(test_package));
}