#ifndef	JSON_RESPONSE_WRITER_HPP
#define	JSON_RESPONSE_WRITER_HPP

#include <functional>
#include <stdexcept>
#include <vector>

#include "json/value.h"

#include "ipc_data_writer.hpp"
#include "clonable_ipc_data_writer.hpp"

namespace ipc {
	template <typename IpcData>
	class JsonIpcDataWriter: public ClonableIpcDataWriter<IpcData> {
	public:
		using RawData = std::vector<char>;
		using IpcDataToJsonDataTransformer = std::function<Json::Value(const IpcData&)>;

		JsonIpcDataWriter(
			const ClonableIpcDataWriter<RawData>& raw_data_writer,
			const IpcDataToJsonDataTransformer& ipc_data_transformer
		);
		JsonIpcDataWriter(const JsonIpcDataWriter&) = default;
		JsonIpcDataWriter& operator=(const JsonIpcDataWriter&) = default;
		void write(const IpcData& response) const override;
		IpcDataWriter<IpcData> *clone() const override;
	private:
		std::shared_ptr<IpcDataWriter<RawData>> m_raw_data_writer;
		IpcDataToJsonDataTransformer m_ipc_data_transformer;
	};

	template <typename IpcData>
	inline JsonIpcDataWriter<IpcData>::JsonIpcDataWriter(
		const ClonableIpcDataWriter<RawData>& raw_data_writer,
		const IpcDataToJsonDataTransformer& ipc_data_transformer
	): m_raw_data_writer(raw_data_writer.clone()), m_ipc_data_transformer(ipc_data_transformer) {
		if (!m_ipc_data_transformer) {
			throw std::invalid_argument("invalid ipc data transformer received");
		}
	}

	template <typename IpcData>
	inline void JsonIpcDataWriter<IpcData>::write(const IpcData& response) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	template <typename IpcData>
	inline IpcDataWriter<IpcData> *JsonIpcDataWriter<IpcData>::clone() const {
		return new JsonIpcDataWriter(*this);
	}
}

#endif // JSON_RESPONSE_WRITER_HPP