#ifndef	STEPPER_IPC_DATA_READER_HPP
#define	STEPPER_IPC_DATA_READER_HPP

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "json/reader.h"
#include "json/value.h"

#include "ipc_data_reader.hpp"
#include "clonable_ipc_data_reader.hpp"
#include "stepper_motor_request.hpp"

namespace ipc {
	class StepperIpcDataReader: public ClonableIpcDataReader<manager::StepperMotorRequest> {
	public:
		using RawData = std::vector<char>;
		using JsonDataToIpcDataTransformer = std::function<manager::StepperMotorRequest(const Json::Value&)>;
		
		StepperIpcDataReader(
			const ClonableIpcDataReader<RawData>& raw_data_reader,
			const JsonDataToIpcDataTransformer& ipc_data_transformer
		);
		StepperIpcDataReader(const StepperIpcDataReader&) = default;
		StepperIpcDataReader& operator=(const StepperIpcDataReader&) = delete;
		std::optional<manager::StepperMotorRequest> read() override;
		IpcDataReader<manager::StepperMotorRequest> *clone() const override;
	private:
		std::shared_ptr<IpcDataReader<RawData>> m_raw_data_reader;
		const JsonDataToIpcDataTransformer m_ipc_data_transformer;

		static Json::Value parse_raw_data(const RawData& data);
	};

	inline StepperIpcDataReader::StepperIpcDataReader(
		const ClonableIpcDataReader<RawData>& raw_data_reader,
		const JsonDataToIpcDataTransformer& ipc_data_transformer
	): m_raw_data_reader(raw_data_reader.clone()), m_ipc_data_transformer(ipc_data_transformer) {
		if (!m_ipc_data_transformer) {
			throw std::invalid_argument("invalid ipc data transformer received");
		}
	}

	inline std::optional<manager::StepperMotorRequest> StepperIpcDataReader::read() {
		const auto raw_data = m_raw_data_reader->read();
		if (!raw_data) {
			return std::optional<manager::StepperMotorRequest>();
		}
		const auto json_data = parse_raw_data(*raw_data);
		const auto parsed_ipc_data = m_ipc_data_transformer(json_data);
		return std::optional<manager::StepperMotorRequest>(parsed_ipc_data);
	}

	inline IpcDataReader<manager::StepperMotorRequest> *StepperIpcDataReader::clone() const {
		return new StepperIpcDataReader(*this);
	}

	
	inline Json::Value StepperIpcDataReader::parse_raw_data(const RawData& data) {
		Json::Value root;
	    Json::Reader reader;
		if (!reader.parse(std::string(data.begin(), data.end()), std::ref(root), true)) {
			throw std::runtime_error("failed to parse raw ipc_data data: " + reader.getFormattedErrorMessages());
		}
		return root;
	}
}

#endif // STEPPER_IPC_DATA_READER_HPP