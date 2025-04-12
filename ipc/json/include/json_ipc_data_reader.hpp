#ifndef	JSON_IPC_DATA_READER_HPP
#define	JSON_IPC_DATA_READER_HPP

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "json/reader.h"
#include "json/value.h"

#include "ipc_data_reader.hpp"
#include "clonable_ipc_data_reader.hpp"

namespace ipc {
	template <typename IpcData>
	class JsonIpcDataReader: public ClonableIpcDataReader<IpcData> {
	public:
		using RawData = std::vector<char>;
		using JsonDataToIpcDataTransformer = std::function<IpcData(const Json::Value&)>;
		
		JsonIpcDataReader(
			const ClonableIpcDataReader<RawData>& raw_data_reader,
			const JsonDataToIpcDataTransformer& ipc_data_transformer
		);
		JsonIpcDataReader(const JsonIpcDataReader&) = default;
		JsonIpcDataReader& operator=(const JsonIpcDataReader&) = default;
		std::optional<IpcData> read() override;
		IpcDataReader<IpcData> *clone() const override;
	private:
		std::shared_ptr<IpcDataReader<RawData>> m_raw_data_reader;
		const JsonDataToIpcDataTransformer m_ipc_data_transformer;

		static Json::Value parse_raw_data(const RawData& data);
	};

	template <typename IpcData>
	inline JsonIpcDataReader<IpcData>::JsonIpcDataReader(
		const ClonableIpcDataReader<RawData>& raw_data_reader,
		const JsonDataToIpcDataTransformer& ipc_data_transformer
	): m_raw_data_reader(raw_data_reader.clone()), m_ipc_data_transformer(ipc_data_transformer) {
		if (!m_ipc_data_transformer) {
			throw std::invalid_argument("invalid ipc data transformer received");
		}
	}

	template <typename IpcData>
	inline std::optional<IpcData> JsonIpcDataReader<IpcData>::read() {
		const auto raw_data = m_raw_data_reader->read();
		if (!raw_data) {
			return std::optional<IpcData>();
		}
		const auto json_data = parse_raw_data(*raw_data);
		const auto parsed_ipc_data = m_ipc_data_transformer(json_data);
		return std::optional<IpcData>(parsed_ipc_data);
	}

	template <typename IpcData>
	inline IpcDataReader<IpcData> *JsonIpcDataReader<IpcData>::clone() const {
		return new JsonIpcDataReader(*this);
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