#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "axes_controller.hpp"
#include "ipc_instance.hpp"
#include "manager_instance.hpp"
#include "movement_host_builder.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"

using namespace ipc;
using namespace manager;
using namespace host;

using AxesConfig = std::string;
using RawData = std::vector<char>;

TEST(ut_movement_host_builder, ctor_dtor_sanity) {
	// WHEN
	MovementHostBuilder<AxesConfig, RawData> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			instance = new MovementHostBuilder<AxesConfig, RawData>()
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_host_builder, build_sanity) {
	// GIVEN
	const auto raw_data_reader = MovementHostBuilder<AxesConfig, RawData>::RawDataReaderInstance(
		new TestIpcDataReader<RawData>(
			[]() -> std::optional<ipc::Instance<RawData>> {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto raw_data_writer = MovementHostBuilder<AxesConfig, RawData>::RawDataWriterInstance(
		new TestIpcDataWriter<RawData>(
			[](const RawData&) {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	const auto api_request_parser = [](const RawData&) -> ipc::Instance<MovementHostBuilder<AxesConfig, RawData>::ApiRequest> {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	const auto api_response_serializer = [](const MovementHostBuilder<AxesConfig, RawData>::ApiResponse&) -> RawData {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	const auto axes_controller_ctor = [](const AxesConfig&) -> manager::Instance<AxesController> {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	
	// WHEN
	MovementHostBuilder<AxesConfig, RawData> instance;
	instance
		.set_raw_data_reader(raw_data_reader)
		.set_api_request_parser(api_request_parser)
		.set_raw_data_writer(raw_data_writer)
		.set_api_response_serializer(api_response_serializer)
		.set_axes_controller_creator(axes_controller_ctor);

	// THEN
	ASSERT_NO_THROW(
		{
			const auto host = instance.build();
		}
	);
}