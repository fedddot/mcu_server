#include <cstddef>
#include <string>

#include "gtest/gtest.h"
#include <vector>

#include "zlib_decompressor.hpp"
#include "zlib_compressor.hpp"

using namespace mcu_server_utl;


TEST(ut_zlib_compressor, some_sanity) {
	// GIVEN
	const std::string test_data_str("test_datatest");
    const std::vector<char> test_data(test_data_str.begin(), test_data_str.end());

	// WHEN
	ZlibCompressor compressor(9);

	// THEN
	auto result = compressor.transform(test_data);
	ASSERT_EQ(test_data, ZlibDecompressor().transform(result, test_data.size()));
}