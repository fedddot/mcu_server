#ifndef	MOVEMENT_RESPONSE_WRITER_HPP
#define	MOVEMENT_RESPONSE_WRITER_HPP

#include <vector>

#include "clonable_ipc_data_writer.hpp"
#include "ipc_data_writer.hpp"
#include "json_ipc_data_writer.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_manager_response.hpp"

namespace ipc {
	class MovementIpcDataWriter: public ClonableIpcDataWriter<manager::MovementManagerResponse> {
	public:
		using RawData = std::vector<char>;
		MovementIpcDataWriter(const ClonableIpcDataWriter<RawData>& raw_data_writer);
		MovementIpcDataWriter(const MovementIpcDataWriter&) = default;
		MovementIpcDataWriter& operator=(const MovementIpcDataWriter&) = default;
		void write(const manager::MovementManagerResponse& ipc_data) const override;
		IpcDataWriter<manager::MovementManagerResponse> *clone() const override;
	private:
		JsonIpcDataWriter<manager::MovementManagerResponse> m_json_data_writer;
	};
	
	inline MovementIpcDataWriter::MovementIpcDataWriter(const ClonableIpcDataWriter<RawData>& raw_data_writer): m_json_data_writer(raw_data_writer, movement_response_to_json_value) {}

	inline void MovementIpcDataWriter::write(const manager::MovementManagerResponse& ipc_data) const {
		m_json_data_writer.write(ipc_data);
	}
	
	inline IpcDataWriter<manager::MovementManagerResponse> *MovementIpcDataWriter::clone() const {
		return new MovementIpcDataWriter(*this);
	}
}

#endif // MOVEMENT_RESPONSE_WRITER_HPP