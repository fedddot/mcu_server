#ifndef	MOVEMENT_IPC_DATA_READER_HPP
#define	MOVEMENT_IPC_DATA_READER_HPP

#include <memory>
#include <optional>

#include "json/value.h"

#include "ipc_clonable.hpp"
#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_result.hpp"
#include "json_ipc_data_reader.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_manager_request.hpp"

namespace ipc {
	template <typename AxisControllerConfig>
	class MovementIpcDataReader: public IpcDataReader<manager::MovementManagerRequest>, public Clonable<IpcDataReader<manager::MovementManagerRequest>> {
	public:
		using AxisControllerConfigToJsonTransformer = typename DefaultMovementDataTransformers<AxisControllerConfig>::AxisControllerConfigToJsonTransformer;
		using JsonToAxisControllerConfigTransformer = typename DefaultMovementDataTransformers<AxisControllerConfig>::JsonToAxisControllerConfigTransformer;
		
		MovementIpcDataReader(
			const Clonable<IpcDataReader<RawData>>& raw_data_reader,
			const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json,
			const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr
		);
		MovementIpcDataReader(const MovementIpcDataReader&) = default;
		MovementIpcDataReader& operator=(const MovementIpcDataReader&) = default;
		std::optional<Result<manager::MovementManagerRequest>> read() override;
		IpcDataReader<manager::MovementManagerRequest> *clone() const override;
	private:
		DefaultMovementDataTransformers<AxisControllerConfig> m_data_transformers;
		std::shared_ptr<IpcDataReader<manager::MovementManagerRequest>> m_data_reader;
	};

	template <typename AxisControllerConfig>
	inline MovementIpcDataReader<AxisControllerConfig>::MovementIpcDataReader(
		const Clonable<IpcDataReader<RawData>>& raw_data_reader,
		const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json,
		const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr
	): m_data_transformers(ctrlr_cfg_to_json, json_cfg_to_ctrlr), m_data_reader(nullptr) {
		m_data_reader = std::shared_ptr<IpcDataReader<manager::MovementManagerRequest>>(
			new JsonIpcDataReader<manager::MovementManagerRequest>(
				raw_data_reader,
				[this](const Json::Value& val) -> Result<manager::MovementManagerRequest> {
					return m_data_transformers.json_value_to_request(val);
				}
			)
		);
	}

	template <typename AxisControllerConfig>
	inline std::optional<Result<manager::MovementManagerRequest>> MovementIpcDataReader<AxisControllerConfig>::read() {
		return m_data_reader->read();
	}

	template <typename AxisControllerConfig>
	inline IpcDataReader<manager::MovementManagerRequest> *MovementIpcDataReader<AxisControllerConfig>::clone() const {
		return new MovementIpcDataReader(*this);
	}
}

#endif // MOVEMENT_IPC_DATA_READER_HPP