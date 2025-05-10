#ifndef	MOVEMENT_RESPONSE_WRITER_HPP
#define	MOVEMENT_RESPONSE_WRITER_HPP

#include <vector>

#include "ipc_instance.hpp"
#include "ipc_data_writer.hpp"
#include "json_ipc_data_writer.hpp"
#include "response_json_transformer.hpp"
#include "movement_manager_response.hpp"

namespace ipc {
	template <typename AxisControllerConfig>
	class MovementIpcDataWriter: public IpcDataWriter<manager::MovementManagerResponse> {
	public:
		using AxisControllerConfigToJsonTransformer = typename ResponseJsonTransformer<AxisControllerConfig>::AxisControllerConfigToJsonTransformer;
		using JsonToAxisControllerConfigTransformer = typename ResponseJsonTransformer<AxisControllerConfig>::JsonToAxisControllerConfigTransformer;
	
		MovementIpcDataWriter(const Clonable<IpcDataWriter<RawData>>& raw_data_writer);
		MovementIpcDataWriter(const MovementIpcDataWriter&) = default;
		MovementIpcDataWriter& operator=(const MovementIpcDataWriter&) = default;
		void write(const manager::MovementManagerResponse& ipc_data) const override;
		IpcDataWriter<manager::MovementManagerResponse> *clone() const override;
	private:
		JsonIpcDataWriter<manager::MovementManagerResponse> m_json_data_writer;
	};
	
	template <typename AxisControllerConfig>
	inline MovementIpcDataWriter::MovementIpcDataWriter(const Clonable<IpcDataWriter<RawData>>& raw_data_writer): m_json_data_writer(raw_data_writer, movement_response_to_json_value) {}

	template <typename AxisControllerConfig>
	inline void MovementIpcDataWriter::write(const manager::MovementManagerResponse& ipc_data) const {
		m_json_data_writer.write(ipc_data);
	}
	
	inline IpcDataWriter<manager::MovementManagerResponse> *MovementIpcDataWriter::clone() const {
		return new MovementIpcDataWriter(*this);
	}
}

#endif // MOVEMENT_RESPONSE_WRITER_HPP