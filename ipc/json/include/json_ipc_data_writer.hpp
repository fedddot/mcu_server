#ifndef	JSON_RESPONSE_WRITER_HPP
#define	JSON_RESPONSE_WRITER_HPP

#include <stdexcept>

#include "ipc_data_writer.hpp"
#include "clonable_ipc_data_writer.hpp"

namespace ipc {
	template <typename IpcData>
	class JsonIpcDataWriter: public ClonableIpcDataWriter<IpcData> {
	public:
		JsonIpcDataWriter(
		);
		JsonIpcDataWriter(const JsonIpcDataWriter&) = default;
		JsonIpcDataWriter& operator=(const JsonIpcDataWriter&) = default;
		void write(const IpcData& response) const override;
		IpcDataWriter<IpcData> *clone() const override;
	private:
	};

	template <typename IpcData>
	inline JsonIpcDataWriter<IpcData>::JsonIpcDataWriter(
	) {
		throw std::runtime_error("NOT IMPLEMENTED");
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