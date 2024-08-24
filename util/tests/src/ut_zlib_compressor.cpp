#include <cstddef>
#include <string>

#include "gtest/gtest.h"
#include <vector>
#include "zlib.h"
#include "zlib_compressor.hpp"

using namespace mcu_server_utl;

std::vector<char> uncompressData(const std::vector<char>& data, const std::size_t& uncompressed_data_size) {
	std::unique_ptr<Bytef[]> data_buff(new Bytef[data.size()]);
	if (!std::copy(data.begin(), data.end(), data_buff.get())) {
		throw std::runtime_error("failed to copy data");
	}
	std::unique_ptr<Bytef[]> uncompressed_data_buff(new Bytef[uncompressed_data_size]);
	uLong actual_uncompressed_data_size(uncompressed_data_size);
	if (Z_OK != uncompress(uncompressed_data_buff.get(), &actual_uncompressed_data_size, data_buff.get(), data.size())) {
		throw std::runtime_error("failed to uncompress data");
	}
	return std::vector<char>(uncompressed_data_buff.get(), uncompressed_data_buff.get() + actual_uncompressed_data_size);
}

TEST(ut_zlib_compressor, some_sanity) {
	// GIVEN
	const std::string test_data_str("test_datatest");
    const std::vector<char> test_data(test_data_str.begin(), test_data_str.end());

	// WHEN
	ZlibCompressor compressor(9);

	// THEN
	auto result = compressor.transform(test_data);
	ASSERT_EQ(test_data, uncompressData(result, test_data.size()));
}