#ifndef FILE_RAW_DATA_READER_HPP
#define FILE_RAW_DATA_READER_HPP

#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>

#include "json/writer.h"
#include "json/reader.h"
#include "json/value.h"

#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"

namespace ipc {
    class FileRawDataReader: public IpcDataReader<RawData> {
    public:
        FileRawDataReader(const std::filesystem::path& data_path);
        FileRawDataReader(const FileRawDataReader&) = delete;
        FileRawDataReader& operator=(const FileRawDataReader&) = delete;

        std::optional<Instance<RawData>> read() override;
        bool empty() const;
    private:
        Json::Value m_json_values;
    };

    inline FileRawDataReader::FileRawDataReader(const std::filesystem::path& data_path) {
        std::ifstream file(data_path, std::ios::in);
        std::stringstream buffer;
        buffer << file.rdbuf();
        const auto data_str = buffer.str();
        Json::Reader reader;
        Json::Value json_val;
        if (!reader.parse(data_str, json_val, true)) {
            throw std::invalid_argument("failed to parse raw ipc_data data: " + reader.getFormattedErrorMessages());
        }
        if (!json_val.isArray()) {
            throw std::invalid_argument("raw ipc_data data is not an array");
        }
        m_json_values = json_val;
    }

    inline std::optional<Instance<RawData>> FileRawDataReader::read() {
        if (m_json_values.empty()) {
            return std::nullopt;
        }
        auto json_request = Json::Value();
        if (!m_json_values.removeIndex(0, &json_request)) {
            throw std::runtime_error("failed to remove json request");
        }
        Json::StreamWriterBuilder writer_builder;
        const auto serial_str = Json::writeString(writer_builder, json_request);
        return Instance<RawData>(
            new RawData(serial_str.begin(), serial_str.end())
        );
    }

    inline bool FileRawDataReader::empty() const {
        return m_json_values.empty();
    }
} // namespace ipc

#endif // FILE_RAW_DATA_READER_HPP