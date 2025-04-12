#ifndef	JSON_IPC_DATA_READER_HPP
#define	JSON_IPC_DATA_READER_HPP

#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

#include "json/reader.h"
#include "json/value.h"

#include "ipc_data_reader.hpp"
#include "shared_ipc_data_reader.hpp"

namespace ipc {
	template <typename IpcData>
	class JsonIpcDataReader: public IpcDataReader<IpcData> {
	public:
		using RawData = std::vector<char>;
		using IpcDataParserFromJsonData = std::function<IpcData(const Json::Value&)>;
		
		JsonIpcDataReader(
			const SharedIpcDataReader<RawData>& raw_data_reader,
			const IpcDataParserFromJsonData& request_parser
		);
		JsonIpcDataReader(const JsonIpcDataReader&) = delete;
		JsonIpcDataReader& operator=(const JsonIpcDataReader&) = delete;
		std::optional<IpcData> read() override;
	private:
		SharedIpcDataReader<RawData> m_raw_data_reader;
		const IpcDataParserFromJsonData m_request_parser;

		static Json::Value parse_raw_data(const RawData& data);
	};

	template <typename IpcData>
	inline JsonIpcDataReader<IpcData>::JsonIpcDataReader(
		const SharedIpcDataReader<RawData>& raw_data_reader,
		const IpcDataParserFromJsonData& request_parser
	): m_raw_data_reader(raw_data_reader), m_request_parser(request_parser) {
		if (!m_request_parser) {
			throw std::invalid_argument("invalid request parser received");
		}
	}

	template <typename IpcData>
	inline std::optional<IpcData> JsonIpcDataReader<IpcData>::read() {
		const auto raw_data = m_raw_data_reader.read();
		if (!raw_data) {
			return std::optional<IpcData>();
		}
		const auto json_data = parse_raw_data(*raw_data);
		const auto parsed_request = m_request_parser(json_data);
		return std::optional<IpcData>(parsed_request);
	}

	template <typename IpcData>
	Json::Value JsonIpcDataReader<IpcData>::parse_raw_data(const RawData& data) {
		Json::Value root;
	    Json::Reader reader;
		if (!reader.parse(std::string(data.begin(), data.end()), std::ref(root), true)) {
			throw std::runtime_error("failed to parse raw request data: " + reader.getFormattedErrorMessages());
		}
		return root;
	}
}

#endif // JSON_IPC_DATA_READER_HPP