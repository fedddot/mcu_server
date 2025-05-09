#ifndef	MOVEMENT_IPC_DATA_READER_HPP
#define	MOVEMENT_IPC_DATA_READER_HPP

#include <optional>

#include "ipc_clonable.hpp"
#include "ipc_data_reader.hpp"
#include "json_ipc_data_reader.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_manager_request.hpp"

namespace ipc {
	class MovementIpcDataReader: public IpcDataReader<manager::MovementManagerRequest>, public Clonable<IpcDataReader<manager::MovementManagerRequest>> {
	public:
		MovementIpcDataReader(const Clonable<IpcDataReader<RawData>>& raw_data_reader);
		MovementIpcDataReader(const MovementIpcDataReader&) = default;
		MovementIpcDataReader& operator=(const MovementIpcDataReader&) = default;
		std::optional<manager::MovementManagerRequest> read() override;
		IpcDataReader<manager::MovementManagerRequest> *clone() const override;
	private:
		JsonIpcDataReader<manager::MovementManagerRequest> m_json_data_reader;
	};

	inline MovementIpcDataReader::MovementIpcDataReader(const Clonable<IpcDataReader<RawData>>& raw_data_reader): m_json_data_reader(raw_data_reader, json_value_to_movement_request) {}

	inline std::optional<manager::MovementManagerRequest> MovementIpcDataReader::read() {
		return m_json_data_reader.read();
	}

	inline IpcDataReader<manager::MovementManagerRequest> *MovementIpcDataReader::clone() const {
		return new MovementIpcDataReader(*this);
	}
}

#endif // MOVEMENT_IPC_DATA_READER_HPP