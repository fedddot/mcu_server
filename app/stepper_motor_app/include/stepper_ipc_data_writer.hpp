#ifndef	STEPPER_RESPONSE_WRITER_HPP
#define	STEPPER_RESPONSE_WRITER_HPP

#include <vector>

#include "ipc_data_writer.hpp"
#include "json_ipc_data_writer.hpp"
#include "stepper_ipc_data_infra.hpp"
#include "stepper_motor_response.hpp"

namespace ipc {
	class StepperIpcDataWriter: public ClonableIpcDataWriter<manager::StepperMotorResponse> {
	public:
		using RawData = std::vector<char>;
		StepperIpcDataWriter(const ClonableIpcDataWriter<RawData>& raw_data_writer);
		StepperIpcDataWriter(const StepperIpcDataWriter&) = default;
		StepperIpcDataWriter& operator=(const StepperIpcDataWriter&) = default;
		void write(const manager::StepperMotorResponse& ipc_data) const override;
		IpcDataWriter<manager::StepperMotorResponse> *clone() const override;
	private:
		JsonIpcDataWriter<manager::StepperMotorResponse> m_json_data_writer;
	};
	
	inline StepperIpcDataWriter::StepperIpcDataWriter(const ClonableIpcDataWriter<RawData>& raw_data_writer): m_json_data_writer(raw_data_writer, stepper_response_to_json_value) {}

	inline void StepperIpcDataWriter::write(const manager::StepperMotorResponse& ipc_data) const {
		m_json_data_writer.write(ipc_data);
	}
	
	inline IpcDataWriter<manager::StepperMotorResponse> *StepperIpcDataWriter::clone() const {
		return new StepperIpcDataWriter(*this);
	}
}

#endif // STEPPER_RESPONSE_WRITER_HPP