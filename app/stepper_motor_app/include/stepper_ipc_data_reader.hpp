#ifndef	STEPPER_IPC_DATA_READER_HPP
#define	STEPPER_IPC_DATA_READER_HPP

#include <map>
#include <optional>
#include <stdexcept>
#include <vector>

#include "json/value.h"

#include "clonable_ipc_data_reader.hpp"
#include "ipc_data_reader.hpp"
#include "json_ipc_data_reader.hpp"
#include "stepper_motor.hpp"
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
		static manager::StepperMotor::Direction parse_direction(const std::string& direction_str);
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
		return manager::StepperMotorRequest {
			.direction = parse_direction(json_data["direction"].asString()),
			.steps_number = json_data["steps_number"].asUInt64(),
			.step_duration_ms = json_data["step_duration_ms"].asUInt64()
		};
	}

	inline manager::StepperMotor::Direction StepperIpcDataReader::parse_direction(const std::string& direction_str) {
		const std::map<std::string, manager::StepperMotor::Direction> direction_map = {
			{"CW", manager::StepperMotor::Direction::CW},
			{"CCW", manager::StepperMotor::Direction::CCW}
		};
		auto it = direction_map.find(direction_str);
		if (direction_map.end() == it) {
			throw std::invalid_argument("invalid stepper motor direction: " + direction_str);
		}
		return it->second;
	}
}

#endif // STEPPER_IPC_DATA_READER_HPP