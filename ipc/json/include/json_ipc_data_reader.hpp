#ifndef	JSON_IPC_DATA_READER_HPP
#define	JSON_IPC_DATA_READER_HPP

#include <functional>
#include <optional>
#include <stdexcept>

#include "json/reader.h"
#include "json/value.h"

#include "ipc_data_reader.hpp"
#include "ipc_data.hpp"
#include "ipc_instance.hpp"

namespace ipc {
	template <typename IpcData>
	class JsonIpcDataReader: public IpcDataReader<IpcData> {
	public:
		using JsonDataToIpcDataTransformer = std::function<Instance<IpcData>(const Json::Value&)>;

		JsonIpcDataReader(
			const Instance<IpcDataReader<RawData>>& raw_data_reader,
			const JsonDataToIpcDataTransformer& ipc_data_transformer
		);
		JsonIpcDataReader(const JsonIpcDataReader&) = default;
		JsonIpcDataReader& operator=(const JsonIpcDataReader&) = default;
		std::optional<Instance<IpcData>> read() override;
	private:
		Instance<IpcDataReader<RawData>> m_raw_data_reader;
		JsonDataToIpcDataTransformer m_ipc_data_transformer;

		static Json::Value parse_raw_data(const RawData& data);
	};

	template <typename IpcData>
	inline JsonIpcDataReader<IpcData>::JsonIpcDataReader(
		const Instance<IpcDataReader<RawData>>& raw_data_reader,
		const JsonDataToIpcDataTransformer& ipc_data_transformer
	): m_raw_data_reader(raw_data_reader), m_ipc_data_transformer(ipc_data_transformer) {
		if (!m_ipc_data_transformer) {
			throw std::invalid_argument("invalid ipc data transformer received");
		}
	}

	template <typename IpcData>
	inline std::optional<Instance<IpcData>> JsonIpcDataReader<IpcData>::read() {
		const auto raw_data = m_raw_data_reader.get().read();
		if (!raw_data) {
			return std::nullopt;
		}
		const auto json_data = parse_raw_data(raw_data->get());
		const auto parsed_ipc_data = m_ipc_data_transformer(json_data);
		return parsed_ipc_data;
	}

	template <typename IpcData>
	Json::Value JsonIpcDataReader<IpcData>::parse_raw_data(const RawData& data) {
		Json::Value root;
	    Json::Reader reader;
		if (!reader.parse(std::string(data.begin(), data.end()), std::ref(root), true)) {
			throw std::runtime_error("failed to parse raw ipc_data data: " + reader.getFormattedErrorMessages());
		}
		return root;
	}
}

#endif // JSON_IPC_DATA_READER_HPP