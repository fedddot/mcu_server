#include <cstddef>
#include <cstring>

#include "gtest/gtest.h"

#include "ring_buffer.hpp"

using namespace ipc;

TEST(ut_ring_buffer, sanity) {
    // GIVEN
    const auto buff_size = std::size_t(6UL);
    const auto test_data = std::string("abcdef");

    // WHEN
    auto instance = RingBuffer<char, buff_size>();

    // THEN
    for (const auto ch: test_data) {
        ASSERT_NO_THROW(instance.push_back(ch));
    }
}