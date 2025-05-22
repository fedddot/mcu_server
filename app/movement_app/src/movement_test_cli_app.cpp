#include <filesystem>
#include <stdexcept>
#include <vector>

#include "ipc_data_reader.hpp"
#include "movement_host_builder.hpp"
#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "raw_data_package_reader.hpp"

using namespace host;
using namespace ipc;
using namespace manager;

class AxesConfig;
using RawData = std::vector<char>;

static AxesConfig parse_axes_config(const RawData& raw_data);
static MovementHostBuilder<AxesConfig, RawData>::RawDataReaderInstance create_raw_data_reader(int argc, char **argv);
static MovementHostBuilder<AxesConfig, RawData>::RawDataWriterInstance create_raw_data_writer();
static MovementHostBuilder<AxesConfig, RawData>::ApiRequestParser create_request_parser();
static MovementHostBuilder<AxesConfig, RawData>::ApiResponseSerializer create_response_serializer();
static MovementHostBuilder<AxesConfig, RawData>::AxesControllerCreator create_axes_controller_creator();

int main(int argc, char **argv) {
	const auto axes_properties = AxesProperties(0.1, 0.2, 0.3);
	MovementHostBuilder<AxesConfig, RawData> instance;
	instance
		.set_raw_data_reader(create_raw_data_reader())
		.set_api_request_parser(create_request_parser())
        .set_raw_data_writer(create_raw_data_writer())
		.set_api_response_serializer(create_response_serializer())
        .set_axes_controller_creator(create_axes_controller_creator())
		.set_axes_properties(axes_properties);

    return 0;
}

class AxesConfig {
public:
    AxesConfig(const std::filesystem::path& output_file_path);
    AxesConfig(const AxesConfig&) = default;
    AxesConfig& operator=(const AxesConfig&) = default;
    virtual ~AxesConfig() noexcept = default;

    std::filesystem::path output_file_path() const;
private:
    std::filesystem::path m_output_file_path;
};

class CmdArgsReader: public IpcDataReader<RawData> {
public:
    CmdArgsReader(int argc, char **argv);
    CmdArgsReader(const CmdArgsReader&) = delete;
    CmdArgsReader& operator=(const CmdArgsReader&) = delete;

    std::optional<ipc::Instance<RawData>> read() override {
        // TODO: read the raw data from argv[1] argument
        // if there is no argument, throw std::runtime_error("no movement request data provided");
        throw std::runtime_error("NOT IMPLEMENTED");
    }
private:
    std::filesystem::path m_file_path;
};

inline MovementHostBuilder<AxesConfig, RawData>::RawDataReaderInstance create_raw_data_reader(int argc, char **argv) {
    return MovementHostBuilder<AxesConfig, RawData>::RawDataReaderInstance(
        new CmdArgsReader(argc, argv)
    );
}