#ifndef	MOVEMENT_HOST_HPP
#define	MOVEMENT_HOST_HPP

#include "axes_controller.hpp"
#include "host.hpp"
#include "ipc_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
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
		MovementHost(
			const ipc::Instance<ipc::IpcDataReader<ipc::RawData>>& ipc_data_reader,
			const ipc::Instance<ipc::IpcDataWriter<ipc::RawData>>& ipc_data_writer,
			const manager::AxesController& axes_controller,
			const manager::AxesProperties& axes_properties
		);
		MovementHost(const MovementHost&) = delete;
		MovementHost& operator=(const MovementHost&) = delete;
	};

	template <typename AxisControllerConfig>
	inline MovementHost<AxisControllerConfig>::MovementHost(
		const ipc::Instance<ipc::IpcDataReader<ipc::RawData>>& ipc_data_reader,
		const ipc::Instance<ipc::IpcDataWriter<ipc::RawData>>& ipc_data_writer,
		const manager::AxesController& axes_controller,
		const manager::AxesProperties& axes_properties
	): Host(
		ipc::Instance<ipc::IpcDataReader<manager::MovementManagerRequest>>(
			new ipc::MovementIpcDataReader<AxisControllerConfig>(ipc_data_reader)
		),
		ipc::Instance<ipc::IpcDataWriter<manager::MovementManagerResponse>>(
			new ipc::MovementIpcDataWriter(ipc_data_writer)
		),
		manager::MovementManager<AxisControllerConfig>(axes_controller, axes_properties),
		[](const std::exception& e) {
			return manager::MovementManagerResponse {
				manager::MovementManagerResponse::ResultCode::EXCEPTION,
				std::string(e.what()),
			};
		}
	) {}
}

#endif // MOVEMENT_HOST_HPP