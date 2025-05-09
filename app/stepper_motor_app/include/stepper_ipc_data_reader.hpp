#ifndef	STEPPER_IPC_DATA_READER_HPP
#define	STEPPER_IPC_DATA_READER_HPP

#include <optional>

#include "ipc_data_reader.hpp"
#include "json_ipc_data_reader.hpp"
#include "stepper_ipc_data_infra.hpp"
#include "stepper_motor_request.hpp"

namespace ipc {
	class StepperIpcDataReader: public ClonableIpcDataReader<manager::StepperMotorRequest> {
	public:
		StepperIpcDataReader(const ClonableIpcDataReader<RawData>& raw_data_reader);
		StepperIpcDataReader(const StepperIpcDataReader&) = default;
		StepperIpcDataReader& operator=(const StepperIpcDataReader&) = default;
		std::optional<manager::StepperMotorRequest> read() override;
		IpcDataReader<manager::StepperMotorRequest> *clone() const override;
	private:
		JsonIpcDataReader<manager::StepperMotorRequest> m_json_data_reader;
	};

	inline StepperIpcDataReader::StepperIpcDataReader(const ClonableIpcDataReader<RawData>& raw_data_reader): m_json_data_reader(raw_data_reader, json_value_to_stepper_request) {}

	inline std::optional<manager::StepperMotorRequest> StepperIpcDataReader::read() {
		return m_json_data_reader.read();
	}

	inline IpcDataReader<manager::StepperMotorRequest> *StepperIpcDataReader::clone() const {
		return new StepperIpcDataReader(*this);
	}
}

#endif // STEPPER_IPC_DATA_READER_HPP