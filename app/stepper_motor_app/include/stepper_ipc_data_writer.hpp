#ifndef	STEPPER_RESPONSE_WRITER_HPP
#define	STEPPER_RESPONSE_WRITER_HPP

#include <vector>

#include "json/value.h"
#include "json/writer.h"

#include "clonable_ipc_data_writer.hpp"
#include "ipc_data_writer.hpp"
#include "json_ipc_data_writer.hpp"
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
		static Json::Value response_to_json(const manager::StepperMotorResponse& response);
	};
	
	inline StepperIpcDataWriter::StepperIpcDataWriter(const ClonableIpcDataWriter<RawData>& raw_data_writer): m_json_data_writer(raw_data_writer, response_to_json) {}

	inline void StepperIpcDataWriter::write(const manager::StepperMotorResponse& ipc_data) const {
		m_json_data_writer.write(ipc_data);
	}
	
	inline IpcDataWriter<manager::StepperMotorResponse> *StepperIpcDataWriter::clone() const {
		return new StepperIpcDataWriter(*this);
	}

	inline Json::Value StepperIpcDataWriter::response_to_json(const manager::StepperMotorResponse& response) {
		Json::Value json_data;
		json_data["result"] = static_cast<int>(response.code());
		if (response.state()) {
			json_data["state"] = static_cast<int>(response.state().value());
		}
		if (response.message()) {
			json_data["message"] = response.message().value();
		}
		return json_data;
	}
}

#endif // STEPPER_RESPONSE_WRITER_HPP