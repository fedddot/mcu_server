#ifndef IPC_READER_BUILDER_HPP
#define IPC_READER_BUILDER_HPP

#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"

namespace ipc {
    template <typename IpcData>
    class IpcReaderBuilder {
    public:
        virtual ~IpcReaderBuilder() noexcept = default;
        virtual Instance<IpcDataReader<IpcData>> build() const = 0;
    };
}

#endif // IPC_READER_BUILDER_HPP