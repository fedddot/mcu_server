#ifndef	MOVEMENT_HOST_HPP
#define	MOVEMENT_HOST_HPP

#include "host.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_ipc_data_reader.hpp"
#include "movement_ipc_data_writer.hpp"
#include "movement_motor_manager.hpp"
#include "movement_motor_request.hpp"
#include "movement_motor_response.hpp"

namespace host {
	class StepperHost: public Host<manager::MovementManagerRequest, manager::MovementManagerResponse> {
	public:
		StepperHost(
			const ipc::ClonableIpcDataReader<ipc::RawData>& ipc_data_reader,
			const ipc::ClonableIpcDataWriter<ipc::RawData>& ipc_data_writer,
			const manager::StepperMotorManager::SteppersCreator& movement_motor_creator,
			const manager::StepperMotorManager::DelayGenerator& delay_generator
		);
		StepperHost(const StepperHost&) = delete;
		StepperHost& operator=(const StepperHost&) = delete;
	};

	inline StepperHost::StepperHost(
		const ipc::ClonableIpcDataReader<ipc::RawData>& ipc_data_reader,
		const ipc::ClonableIpcDataWriter<ipc::RawData>& ipc_data_writer,
		const manager::StepperMotorManager::SteppersCreator& movement_motor_creator,
		const manager::StepperMotorManager::DelayGenerator& delay_generator
	): Host(
		ipc::StepperIpcDataReader(ipc_data_reader),
		ipc::StepperIpcDataWriter(ipc_data_writer),
		manager::StepperMotorManager(movement_motor_creator, delay_generator),
		[](const std::exception& e) {
			return manager::MovementManagerResponse {
				manager::MovementManagerResponse::ResultCode::EXCEPTION,
				std::nullopt,
				std::string(e.what())
			};
		}
	) {}
}

#endif // MOVEMENT_HOST_HPP