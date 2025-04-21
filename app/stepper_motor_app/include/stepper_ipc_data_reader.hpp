#ifndef	STEPPER_IPC_DATA_READER_HPP
#define	STEPPER_IPC_DATA_READER_HPP

#include <optional>
#include <stdexcept>
#include <vector>

#include "json/value.h"

#include "clonable_ipc_data_reader.hpp"
#include "ipc_data_reader.hpp"
#include "json_ipc_data_reader.hpp"
#include "stepper_motor_request.hpp"

namespace ipc {
	class StepperIpcDataReader: public ClonableIpcDataReader<manager::StepperMotorRequest> {
	public:
		using RawData = std::vector<char>;

		StepperIpcDataReader(
			const ClonableIpcDataReader<RawData>& raw_data_reader
		);
		StepperIpcDataReader(const StepperIpcDataReader&) = default;
		StepperIpcDataReader& operator=(const StepperIpcDataReader&) = default;
		std::optional<manager::StepperMotorRequest> read() override;
		IpcDataReader<manager::StepperMotorRequest> *clone() const override;
	private:
		JsonIpcDataReader<manager::StepperMotorRequest> m_json_data_reader;
		static manager::StepperMotorRequest parse_json_data(const Json::Value& json_data);
	};

	inline StepperIpcDataReader::StepperIpcDataReader(
		const ClonableIpcDataReader<RawData>& raw_data_reader
	): m_json_data_reader(raw_data_reader, parse_json_data) {

	}

	inline std::optional<manager::StepperMotorRequest> StepperIpcDataReader::read() {
		return m_json_data_reader.read();
	}

	inline IpcDataReader<manager::StepperMotorRequest> *StepperIpcDataReader::clone() const {
		return new StepperIpcDataReader(*this);
	}
	
	inline manager::StepperMotorRequest StepperIpcDataReader::parse_json_data(const Json::Value& json_data) {
		throw std::runtime_error("not implemented");
	}
}

#endif // STEPPER_IPC_DATA_READER_HPP