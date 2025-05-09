#ifndef	JSON_RESPONSE_WRITER_HPP
#define	JSON_RESPONSE_WRITER_HPP

#include <functional>
#include <stdexcept>

#include "json/value.h"
#include "json/writer.h"

#include "ipc_clonable.hpp"
#include "ipc_data_writer.hpp"
#include "ipc_data.hpp"

namespace ipc {
	template <typename IpcData>
	class JsonIpcDataWriter: public IpcDataWriter<IpcData>, public Clonable<IpcDataWriter<IpcData>> {
	public:
		using IpcDataToJsonDataTransformer = std::function<Json::Value(const IpcData&)>;
		using ClonableRawDataWriter = Clonable<IpcDataWriter<RawData>>;

		JsonIpcDataWriter(
			const ClonableRawDataWriter& raw_data_writer,
			const IpcDataToJsonDataTransformer& ipc_data_transformer
		);
		JsonIpcDataWriter(const JsonIpcDataWriter&) = default;
		JsonIpcDataWriter& operator=(const JsonIpcDataWriter&) = default;
		void write(const IpcData& ipc_data) const override;
		IpcDataWriter<IpcData> *clone() const override;
	private:
		std::shared_ptr<IpcDataWriter<RawData>> m_raw_data_writer;
		IpcDataToJsonDataTransformer m_ipc_data_transformer;
	};

	template <typename IpcData>
	inline JsonIpcDataWriter<IpcData>::JsonIpcDataWriter(
		const ClonableRawDataWriter& raw_data_writer,
		const IpcDataToJsonDataTransformer& ipc_data_transformer
	): m_raw_data_writer(raw_data_writer.clone()), m_ipc_data_transformer(ipc_data_transformer) {
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
		m_raw_data_writer->write(raw_serial_data);
	}

	template <typename IpcData>
	inline IpcDataWriter<IpcData> *JsonIpcDataWriter<IpcData>::clone() const {
		return new JsonIpcDataWriter(*this);
	}
}

#endif // JSON_RESPONSE_WRITER_HPP