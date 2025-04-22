#ifndef	STEPPER_HOST_HPP
#define	STEPPER_HOST_HPP

#include "custom_manager.hpp"
#include "host.hpp"
#include "stepper_ipc_data_infra.hpp"
#include "stepper_ipc_data_reader.hpp"
#include "stepper_ipc_data_writer.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"

namespace host {
	class StepperHost: public Host<manager::StepperMotorRequest, manager::StepperMotorResponse> {
	public:
		StepperHost(
			const ipc::ClonableIpcDataReader<ipc::RawData>& ipc_data_reader,
			const ipc::ClonableIpcDataWriter<ipc::RawData>& ipc_data_writer
		);
		StepperHost(const StepperHost&) = delete;
		StepperHost& operator=(const StepperHost&) = delete;
	private:
		static manager::CustomManager<manager::StepperMotorRequest, manager::StepperMotorResponse> create_manager();
	};

	inline StepperHost::StepperHost(
		const ipc::ClonableIpcDataReader<ipc::RawData>& ipc_data_reader,
		const ipc::ClonableIpcDataWriter<ipc::RawData>& ipc_data_writer
	): Host(
		ipc::StepperIpcDataReader(ipc_data_reader),
		ipc::StepperIpcDataWriter(ipc_data_writer),
		create_manager(),
		[](const std::exception& e) {
			return manager::StepperMotorResponse(manager::StepperMotorResponse::ResultCode::EXCEPTION);
		}
	) {}

	inline manager::CustomManager<manager::StepperMotorRequest, manager::StepperMotorResponse> StepperHost::create_manager() {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // STEPPER_HOST_HPP