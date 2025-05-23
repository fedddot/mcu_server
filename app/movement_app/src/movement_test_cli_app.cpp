#include <chrono>
#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <thread>

#include "json/reader.h"
#include "json/writer.h"
#include "json/value.h"

#include "axes_controller.hpp"
#include "file_raw_data_reader.hpp"
#include "ipc_data.hpp"
#include "movement_host_builder.hpp"
#include "movement_json_api_request_parser.hpp"
#include "movement_json_api_response_serializer.hpp"
#include "movement_manager_data.hpp"
#include "movement_vendor_api_response.hpp"

using namespace host;
using namespace ipc;
using namespace manager;
using namespace vendor;

class AxesConfig;
using RawData = ipc::RawData;

static MovementHostBuilder<AxesConfig, RawData>::RawDataReaderInstance create_raw_data_reader(const std::filesystem::path& data_path);
static MovementHostBuilder<AxesConfig, RawData>::RawDataWriterInstance create_raw_data_writer();
static MovementHostBuilder<AxesConfig, RawData>::ApiRequestParser create_request_parser();
static MovementHostBuilder<AxesConfig, RawData>::ApiResponseSerializer create_response_serializer();
static MovementHostBuilder<AxesConfig, RawData>::AxesControllerCreator create_axes_controller_creator();

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_json_file>" << std::endl;
        return 1;
    }
    const auto json_file_path = std::filesystem::path(argv[1]);
    const auto raw_data_reader = create_raw_data_reader(std::filesystem::path(json_file_path));
    const auto& file_raw_data_reader = dynamic_cast<const FileRawDataReader&>(raw_data_reader.get());
	MovementHostBuilder<AxesConfig, RawData> host_builder;
	host_builder
		.set_raw_data_reader(raw_data_reader)
		.set_api_request_parser(create_request_parser())
        .set_raw_data_writer(create_raw_data_writer())
		.set_api_response_serializer(create_response_serializer())
        .set_axes_controller_creator(create_axes_controller_creator());
    auto host = host_builder.build();
    while (!file_raw_data_reader.empty()) {
        host.run_once();
    }
    return 0;
}

class AxesConfig {
public:
    AxesConfig(
        const double x_step_length,
        const double y_step_length,
        const double z_step_length
    ): m_axes_step_lengths({
        {Axis::X, x_step_length},
        {Axis::Y, y_step_length},
        {Axis::Z, z_step_length}
    }) {    
        for (const auto& [_, step_length] : m_axes_step_lengths) {
            if (step_length <= 0.0) {
                throw std::invalid_argument("step lengths must be positive non-zero values");
            }
        }
    }
    AxesConfig(const AxesConfig&) = default;
    AxesConfig& operator=(const AxesConfig&) = default;
    virtual ~AxesConfig() noexcept = default;

    double get_step_length(const Axis& axis) const {
        return m_axes_step_lengths.at(axis);
    }
private:
    std::map<Axis, double> m_axes_step_lengths;
};

class StdOutWriter: public IpcDataWriter<RawData> {
public:
    StdOutWriter() = default;
    StdOutWriter(const StdOutWriter&) = delete;
    StdOutWriter& operator=(const StdOutWriter&) = delete;

    void write(const RawData& data) const override {
        const auto data_str = std::string(data.begin(), data.end());
        std::cout << data_str << std::endl;
    }
};

class DummyAxesController: public AxesController {
public:
    DummyAxesController(const AxesConfig& axes_config): m_axes_config(axes_config) {

    }
    DummyAxesController(const DummyAxesController&) = delete;
    DummyAxesController& operator=(const DummyAxesController&) = delete;

    void step(const Axis& axis, const Direction& direction, const double duration) override {
        const auto axis_mapping = std::map<Axis, std::string>{
            {Axis::X, "X"},
            {Axis::Y, "Y"},
            {Axis::Z, "Z"},
        };
        const auto direction_mapping = std::map<Direction, std::string>{
            {Direction::POSITIVE, "POSITIVE"},
            {Direction::NEGATIVE, "NEGATIVE"},
        };
        std::cout << "performing step along " << axis_mapping.at(axis) << " axis in " << direction_mapping.at(direction) << " direction with duration = " << duration << " s" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(duration * 1000.0)));
    }
    void enable() override {
        std::cout << "axes controller enabled" << std::endl;
    }
    void disable() override {
        std::cout << "axes controller disabled" << std::endl;
    }
    double get_step_length(const Axis& axis) const override {
        return m_axes_config.get_step_length(axis);
    }
private:
    AxesConfig m_axes_config;
};

inline MovementHostBuilder<AxesConfig, RawData>::RawDataReaderInstance create_raw_data_reader(const std::filesystem::path& data_path) {
    return MovementHostBuilder<AxesConfig, RawData>::RawDataReaderInstance(
        new FileRawDataReader(data_path)
    );
}

inline MovementHostBuilder<AxesConfig, RawData>::RawDataWriterInstance create_raw_data_writer() {
    return MovementHostBuilder<AxesConfig, RawData>::RawDataWriterInstance(
        new StdOutWriter()
    );
}

inline MovementHostBuilder<AxesConfig, RawData>::ApiRequestParser create_request_parser() {
    const auto json_parser = MovementJsonApiRequestParser<AxesConfig>(
        [](const Json::Value& json_data) {
            auto retrieve_double = [](const Json::Value& json_val, const std::string& key) {
                if (!json_val.isMember(key)) {
                    throw std::invalid_argument("missing " + key + " key in json data");
                }
                const auto val = json_val[key];
                if (!val.isDouble()) {
                    throw std::invalid_argument(key + " key in json data is not a double");
                }
                return val.asDouble();
            };
            return AxesConfig(
                retrieve_double(json_data, "x_step_length"),
                retrieve_double(json_data, "y_step_length"),
                retrieve_double(json_data, "z_step_length")
            );
        }
    );
    return [json_parser](const RawData& raw_data) {
        Json::Value json_val;
	    Json::Reader reader;
		if (!reader.parse(std::string(raw_data.begin(), raw_data.end()), std::ref(json_val), true)) {
			throw std::runtime_error("failed to parse raw ipc_data data: " + reader.getFormattedErrorMessages());
		}
        return json_parser(json_val);
    };
}

inline MovementHostBuilder<AxesConfig, RawData>::ApiResponseSerializer create_response_serializer() {
    const auto json_serializer = MovementJsonApiResponseSerializer();
    return [json_serializer](const MovementVendorApiResponse& response) {
        const auto json_val = json_serializer(response);
        const auto writer_builder = Json::StreamWriterBuilder();
		const auto serial_str = Json::writeString(writer_builder, json_val);
		return RawData(serial_str.begin(), serial_str.end());
    };
}

inline MovementHostBuilder<AxesConfig, RawData>::AxesControllerCreator create_axes_controller_creator() {
    return [](const AxesConfig& axes_config) {
        return manager::Instance<AxesController>(new DummyAxesController(axes_config));
    };
}