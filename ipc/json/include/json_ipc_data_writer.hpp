#ifndef	JSON_RESPONSE_WRITER_HPP
#define	JSON_RESPONSE_WRITER_HPP

#include <functional>
#include <stdexcept>

#include "json/value.h"
#include "json/writer.h"

#include "ipc_data_writer.hpp"
#include "ipc_data.hpp"
#include "ipc_instance.hpp"

namespace ipc {
	template <typename IpcData>
	class JsonIpcDataWriter: public IpcDataWriter<IpcData> {
	public:
		using IpcDataToJsonDataTransformer = std::function<Json::Value(const IpcData&)>;
		
		JsonIpcDataWriter(
			const Instance<IpcDataWriter<RawData>>& raw_data_writer,
			const IpcDataToJsonDataTransformer& ipc_data_transformer
		);
		JsonIpcDataWriter(const JsonIpcDataWriter&) = default;
		JsonIpcDataWriter& operator=(const JsonIpcDataWriter&) = default;
		void write(const IpcData& ipc_data) const override;
	private:
		Instance<IpcDataWriter<RawData>> m_raw_data_writer;
		IpcDataToJsonDataTransformer m_ipc_data_transformer;
	};

	template <typename IpcData>
	inline JsonIpcDataWriter<IpcData>::JsonIpcDataWriter(
		const Instance<IpcDataWriter<RawData>>& raw_data_writer,
		const IpcDataToJsonDataTransformer& ipc_data_transformer
	): m_raw_data_writer(raw_data_writer), m_ipc_data_transformer(ipc_data_transformer) {
		if (!m_ipc_data_transformer) {
			throw std::invalid_argument("invalid ipc data transformer received");
		}
	}

	template <typename IpcData>
	inline void JsonIpcDataWriter<IpcData>::write(const IpcData& ipc_data) const {
		const auto json_val = m_ipc_data_transformer(ipc_data);
		const auto writer_builder = Json::StreamWriterBuilder();
		const auto serial_str = Json::writeString(writer_builder, json_val);
		const auto raw_serial_data = RawData(serial_str.begin(), serial_str.end());
		m_raw_data_writer.get().write(raw_serial_data);
	}
}

#endif // JSON_RESPONSE_WRITER_HPP