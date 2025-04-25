#ifndef	MOVEMENT_RESPONSE_WRITER_HPP
#define	MOVEMENT_RESPONSE_WRITER_HPP

#include <vector>

#include "clonable_ipc_data_writer.hpp"
#include "ipc_data_writer.hpp"
#include "json_ipc_data_writer.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_manager_response.hpp"

namespace ipc {
	class StepperIpcDataWriter: public ClonableIpcDataWriter<manager::MovementManagerResponse> {
	public:
		using RawData = std::vector<char>;
		StepperIpcDataWriter(const ClonableIpcDataWriter<RawData>& raw_data_writer);
		StepperIpcDataWriter(const StepperIpcDataWriter&) = default;
		StepperIpcDataWriter& operator=(const StepperIpcDataWriter&) = default;
		void write(const manager::MovementManagerResponse& ipc_data) const override;
		IpcDataWriter<manager::MovementManagerResponse> *clone() const override;
	private:
		JsonIpcDataWriter<manager::MovementManagerResponse> m_json_data_writer;
	};
	
	inline StepperIpcDataWriter::StepperIpcDataWriter(const ClonableIpcDataWriter<RawData>& raw_data_writer): m_json_data_writer(raw_data_writer, movement_response_to_json_value) {}

	inline void StepperIpcDataWriter::write(const manager::MovementManagerResponse& ipc_data) const {
		m_json_data_writer.write(ipc_data);
	}
	
	inline IpcDataWriter<manager::MovementManagerResponse> *StepperIpcDataWriter::clone() const {
		return new StepperIpcDataWriter(*this);
	}
}

#endif // MOVEMENT_RESPONSE_WRITER_HPP