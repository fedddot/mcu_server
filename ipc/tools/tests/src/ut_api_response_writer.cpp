#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "api_response_writer.hpp"
#include "package_utils.hpp"

using namespace ipc;

#define HEADER_SIZE 	0x2UL

using ApiResponse = std::string;
using TestWriter = ApiResponseWriter<ApiResponse, HEADER_SIZE>;

TEST(ut_api_response_writer, ctor_dtor_sanity) {
	// GIVEN
	auto header_generator = [](const std::vector<std::uint8_t>& package_data, const std::size_t& header_size) -> std::vector<std::uint8_t> {
		throw std::runtime_error("not implemented");
	};
	auto raw_data_writer = [](const std::vector<std::uint8_t>& data) {
		throw std::runtime_error("not implemented");
	};
	auto response_serializer = [](const ApiResponse& response) -> std::vector<std::uint8_t> {
		throw std::runtime_error("not implemented");
	};

	// WHEN
	TestWriter *instance = nullptr;


	// THEN
	ASSERT_NO_THROW(
		instance = new TestWriter(
			header_generator,
			raw_data_writer,
			response_serializer
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_api_response_writer, write_sanity) {
	// GIVEN
	const auto api_response = ApiResponse("test_msg");
	
	// WHEN
	auto header_generator = [](const std::vector<std::uint8_t>& package_data, const std::size_t& header_size) -> std::vector<std::uint8_t> {
		return serialize_package_size(package_data.size(), header_size);
	};
	auto raw_data_writer = [api_response](const std::vector<std::uint8_t>& data) {
		ASSERT_FALSE(data.empty());
	};
	auto response_serializer = [](const ApiResponse& response) -> std::vector<std::uint8_t> {
		return std::vector<std::uint8_t>(response.begin(), response.end());
	};
	auto instance = TestWriter(
		header_generator,
		raw_data_writer,
		response_serializer
	);

	// THEN
	ASSERT_NO_THROW(instance.write(api_response));
}