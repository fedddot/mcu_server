#ifndef	MOVEMENT_IPC_DATA_READER_HPP
#define	MOVEMENT_IPC_DATA_READER_HPP

#include <optional>

#include "json/value.h"

#include "ipc_instance.hpp"
#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"
#include "json_ipc_data_reader.hpp"
#include "request_json_transformer.hpp"
#include "movement_manager_request.hpp"

namespace ipc {
	template <typename AxesConfig>
	class MovementIpcDataReader: public IpcDataReader<manager::MovementManagerRequest> {
	public:
		using AxesConfigToJsonTransformer = typename RequestJsonTransformer<AxesConfig>::AxesConfigToJsonTransformer;
		using JsonToAxesConfigTransformer = typename RequestJsonTransformer<AxesConfig>::JsonToAxesConfigTransformer;
		
		MovementIpcDataReader(
			const Instance<IpcDataReader<RawData>>& raw_data_reader,
			const AxesConfigToJsonTransformer& ctrlr_cfg_to_json,
			const JsonToAxesConfigTransformer& json_cfg_to_ctrlr
		);
		MovementIpcDataReader(const MovementIpcDataReader&) = default;
		MovementIpcDataReader& operator=(const MovementIpcDataReader&) = default;
		std::optional<Instance<manager::MovementManagerRequest>> read() override;
	private:
		RequestJsonTransformer<AxesConfig> m_data_transformers;
		Instance<IpcDataReader<manager::MovementManagerRequest>> m_data_reader;
	};

	template <typename AxesConfig>
	inline MovementIpcDataReader<AxesConfig>::MovementIpcDataReader(
		const Instance<IpcDataReader<RawData>>& raw_data_reader,
		const AxesConfigToJsonTransformer& ctrlr_cfg_to_json,
		const JsonToAxesConfigTransformer& json_cfg_to_ctrlr
	):
		m_data_transformers(ctrlr_cfg_to_json, json_cfg_to_ctrlr),
		m_data_reader(new JsonIpcDataReader<manager::MovementManagerRequest>(
			raw_data_reader,
			[this](const Json::Value& val) -> Instance<manager::MovementManagerRequest> {
				return m_data_transformers.json_value_to_request(val);
			}
		)) {
	}

	template <typename AxesConfig>
	inline std::optional<Instance<manager::MovementManagerRequest>> MovementIpcDataReader<AxesConfig>::read() {
		return m_data_reader.get().read();
	}
}

#endif // MOVEMENT_IPC_DATA_READER_HPP