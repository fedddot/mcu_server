#ifndef	MOVEMENT_HOST_HPP
#define	MOVEMENT_HOST_HPP

#include "host.hpp"
#include "ipc_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "manager_instance.hpp"
#include "movement_ipc_data_reader.hpp"
#include "movement_ipc_data_writer.hpp"
#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"

namespace host {
	template <typename AxisControllerConfig>
	class MovementHost: public Host<manager::MovementManagerRequest, manager::MovementManagerResponse> {
	public:
		using AxisControllerConfigToJsonTransformer = typename ipc::RequestJsonTransformer<AxisControllerConfig>::AxisControllerConfigToJsonTransformer;
		using JsonToAxisControllerConfigTransformer = typename ipc::RequestJsonTransformer<AxisControllerConfig>::JsonToAxisControllerConfigTransformer;
		using AxesControllerCreator = typename manager::MovementManager<AxisControllerConfig>::AxesControllerCreator;

		MovementHost(
			const ipc::Instance<ipc::IpcDataReader<ipc::RawData>>& ipc_data_reader,
			const ipc::Instance<ipc::IpcDataWriter<ipc::RawData>>& ipc_data_writer,
			const AxesControllerCreator& axes_controller_ctor,
			const manager::AxesProperties& axes_properties,
			const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json,
			const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr
		);
		MovementHost(const MovementHost&) = delete;
		MovementHost& operator=(const MovementHost&) = delete;
	};

	template <typename AxisControllerConfig>
	inline MovementHost<AxisControllerConfig>::MovementHost(
		const ipc::Instance<ipc::IpcDataReader<ipc::RawData>>& ipc_data_reader,
		const ipc::Instance<ipc::IpcDataWriter<ipc::RawData>>& ipc_data_writer,
		const AxesControllerCreator& axes_controller_ctor,
		const manager::AxesProperties& axes_properties,
		const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json,
		const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr
	): Host(
		ipc::Instance<ipc::IpcDataReader<manager::MovementManagerRequest>>(
			new ipc::MovementIpcDataReader<AxisControllerConfig>(ipc_data_reader, ctrlr_cfg_to_json, json_cfg_to_ctrlr)
		),
		ipc::Instance<ipc::IpcDataWriter<manager::MovementManagerResponse>>(
			new ipc::MovementIpcDataWriter(ipc_data_writer)
		),
		manager::Instance<manager::Manager<manager::MovementManagerRequest, manager::MovementManagerResponse>>(
			new manager::MovementManager<AxisControllerConfig>(axes_controller_ctor, axes_properties)
		),
		[](const std::exception& e) {
			return manager::MovementManagerResponse {
				manager::MovementManagerResponse::ResultCode::EXCEPTION,
				std::string(e.what()),
			};
		}
	) {}
}

#endif // MOVEMENT_HOST_HPP