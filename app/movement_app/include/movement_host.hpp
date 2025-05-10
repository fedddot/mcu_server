#ifndef	MOVEMENT_HOST_HPP
#define	MOVEMENT_HOST_HPP

#include "host.hpp"
#include "ipc_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "response_json_transformer.hpp"
#include "movement_ipc_data_reader.hpp"
#include "movement_ipc_data_writer.hpp"
#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"

namespace host {
	class MovementHost: public Host<manager::MovementManagerRequest, manager::MovementManagerResponse> {
	public:
		MovementHost(
			const ipc::Clonable<ipc::IpcDataReader<ipc::RawData>>& ipc_data_reader,
			const ipc::Clonable<ipc::IpcDataWriter<ipc::RawData>>& ipc_data_writer,
			const manager::MovementManager::AxesController& axes_controller,
			const manager::AxesProperties& axes_properties
		);
		MovementHost(const MovementHost&) = delete;
		MovementHost& operator=(const MovementHost&) = delete;
	};

	inline MovementHost::MovementHost(
		const ipc::Clonable<ipc::IpcDataReader<ipc::RawData>>& ipc_data_reader,
		const ipc::Clonable<ipc::IpcDataWriter<ipc::RawData>>& ipc_data_writer,
		const manager::MovementManager::AxesController& axes_controller,
		const manager::AxesProperties& axes_properties
	): Host(
		ipc::MovementIpcDataReader(ipc_data_reader),
		ipc::MovementIpcDataWriter(ipc_data_writer),
		manager::MovementManager(axes_controller, axes_properties),
		[](const std::exception& e) {
			return manager::MovementManagerResponse {
				manager::MovementManagerResponse::ResultCode::EXCEPTION,
				std::string(e.what()),
			};
		}
	) {}
}

#endif // MOVEMENT_HOST_HPP