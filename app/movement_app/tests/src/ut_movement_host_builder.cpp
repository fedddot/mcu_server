#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/value.h"
#include "axes_controller.hpp"
#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "ipc_instance.hpp"
#include "manager_instance.hpp"
#include "movement_host_builder.hpp"
#include "movement_manager_data.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"

using namespace ipc;
using namespace manager;
using namespace host;

using AxisControllerConfig = std::string;

static Json::Value cfg2json(const AxisControllerConfig& cfg);
static AxisControllerConfig json2cfg(const Json::Value& cfg);

TEST(ut_movement_host_builder, ctor_dtor_sanity) {
	// WHEN
	MovementHostBuilder<AxisControllerConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new MovementHostBuilder<AxisControllerConfig>());
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_host_builder, build_sanity) {
	// WHEN
	MovementHostBuilder<AxisControllerConfig> instance;
	instance.set_axes_properties(AxesProperties(0.1, 0.1, 0.1));
	instance.set_axes_controller_ctor(
		[](const AxisControllerConfig&) -> manager::Instance<AxesController> {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	instance.set_raw_data_writer(
		ipc::Instance<IpcDataWriter<RawData>>(
			new TestIpcDataWriter<RawData>(
				[](const RawData&) {
					throw std::runtime_error("NOT IMPLEMENTED");
				}
			)
		)
	);
	instance.set_raw_data_reader(
		ipc::Instance<IpcDataReader<RawData>>(
			new TestIpcDataReader<RawData>(
				[]() -> std::optional<ipc::Instance<RawData>> {
					throw std::runtime_error("NOT IMPLEMENTED");
				}
			)
		)
	);
	instance.set_json_cfg_to_ctrlr(json2cfg);
	instance.set_ctrlr_cfg_to_json(cfg2json);

	// THEN
	ASSERT_NO_THROW(
		{
			auto host = instance.build();
			host.get().run_once();
		}
	);
}

inline Json::Value cfg2json(const AxisControllerConfig& cfg) {
	return Json::Value(cfg);
}

inline AxisControllerConfig json2cfg(const Json::Value& cfg) {
	return cfg.asString();
}