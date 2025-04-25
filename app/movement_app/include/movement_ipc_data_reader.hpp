#ifndef	MOVEMENT_IPC_DATA_READER_HPP
#define	MOVEMENT_IPC_DATA_READER_HPP

#include <optional>

#include "clonable_ipc_data_reader.hpp"
#include "ipc_data_reader.hpp"
#include "json_ipc_data_reader.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_manager_request.hpp"

namespace ipc {
	class StepperIpcDataReader: public ClonableIpcDataReader<manager::MovementManagerRequest> {
	public:
		StepperIpcDataReader(const ClonableIpcDataReader<RawData>& raw_data_reader);
		StepperIpcDataReader(const StepperIpcDataReader&) = default;
		StepperIpcDataReader& operator=(const StepperIpcDataReader&) = default;
		std::optional<manager::MovementManagerRequest> read() override;
		IpcDataReader<manager::MovementManagerRequest> *clone() const override;
	private:
		JsonIpcDataReader<manager::MovementManagerRequest> m_json_data_reader;
	};

	inline StepperIpcDataReader::StepperIpcDataReader(const ClonableIpcDataReader<RawData>& raw_data_reader): m_json_data_reader(raw_data_reader, json_value_to_movement_request) {}

	inline std::optional<manager::MovementManagerRequest> StepperIpcDataReader::read() {
		return m_json_data_reader.read();
	}

	inline IpcDataReader<manager::MovementManagerRequest> *StepperIpcDataReader::clone() const {
		return new StepperIpcDataReader(*this);
	}
}

#endif // MOVEMENT_IPC_DATA_READER_HPP