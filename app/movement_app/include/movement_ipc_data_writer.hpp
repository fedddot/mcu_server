#ifndef	MOVEMENT_RESPONSE_WRITER_HPP
#define	MOVEMENT_RESPONSE_WRITER_HPP

#include <vector>

#include "ipc_instance.hpp"
#include "ipc_data_writer.hpp"
#include "json_ipc_data_writer.hpp"
#include "response_json_transformer.hpp"
#include "movement_manager_response.hpp"

namespace ipc {
	class MovementIpcDataWriter: public IpcDataWriter<manager::MovementManagerResponse> {
	public:
		MovementIpcDataWriter(const Instance<IpcDataWriter<RawData>>& raw_data_writer);
		MovementIpcDataWriter(const MovementIpcDataWriter&) = default;
		MovementIpcDataWriter& operator=(const MovementIpcDataWriter&) = default;
		void write(const manager::MovementManagerResponse& ipc_data) const override;
	private:
		ResponseJsonTransformer m_response_transformer;
		JsonIpcDataWriter<manager::MovementManagerResponse> m_json_data_writer;
	};
	
	
	inline MovementIpcDataWriter::MovementIpcDataWriter(
		const Instance<IpcDataWriter<RawData>>& raw_data_writer
	): m_json_data_writer(
		raw_data_writer,
		[this](const manager::MovementManagerResponse& response) {
			return m_response_transformer.response_to_json_value(response);
		}
	) {

	}

	inline void MovementIpcDataWriter::write(const manager::MovementManagerResponse& ipc_data) const {
		m_json_data_writer.write(ipc_data);
	}
}

#endif // MOVEMENT_RESPONSE_WRITER_HPP