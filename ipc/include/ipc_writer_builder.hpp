#ifndef IPC_WRITER_BUILDER_HPP
#define IPC_WRITER_BUILDER_HPP

#include "ipc_data_writer.hpp"
#include "ipc_instance.hpp"

namespace ipc {
    template <typename IpcData>
    class IpcWriterBuilder {
    public:
        virtual ~IpcWriterBuilder() noexcept = default;
        virtual Instance<IpcDataWriter<IpcData>> build() const = 0;
    };
}

#endif // IPC_WRITER_BUILDER_HPP