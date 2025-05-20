#ifndef	MOVEMENT_HOST_BUILDER_HPP
#define	MOVEMENT_HOST_BUILDER_HPP

#include <optional>
#include <stdexcept>

#include "host.hpp"
#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "ipc_instance.hpp"
#include "movement_vendor_api_request.hpp"
#include "movement_vendor_api_response.hpp"
#include "raw_data_package_descriptor.hpp"
#include "raw_data_package_reader.hpp"

namespace host {
	template <typename AxesConfig>
	class MovementHostBuilder {
	public:
		using RawData = ipc::RawData;
		using RawDataPackageDescriptor = ipc::RawDataPackageDescriptor;
		using PackageSizeParser = ipc::RawDataPackageReader::PackageSizeParser;
		using RawDataReaderInstance = ipc::Instance<ipc::IpcDataReader<RawData>>;
		using RawDataWriterInstance = ipc::Instance<ipc::IpcDataWriter<RawData>>;
		
		MovementHostBuilder() = default;
		MovementHostBuilder(const MovementHostBuilder&) = default;
		MovementHostBuilder& operator=(const MovementHostBuilder&) = default;
		virtual ~MovementHostBuilder() noexcept = default;
		
		Host<vendor::MovementVendorApiRequest, vendor::MovementVendorApiResponse> build() const {
			// const auto package_reader = ipc::RawDataPackageReader(
			// 	RawData *buffer, const RawDataPackageDescriptor &descriptor, const PackageSizeParser &size_parser
			// )
			throw std::runtime_error("NOT IMPLEMENTED");
		}
		void set_raw_data_buffer_ptr(RawData *buffer);
		void set_package_descriptor(const RawDataPackageDescriptor& descriptor);
		void set_package_size_parser(const PackageSizeParser& size_parser);
		void set_raw_data_reader(const RawDataReaderInstance& raw_data_reader);
		void set_raw_data_writer(const RawDataWriterInstance& raw_data_writer);
		// void set_axes_controller_ctor(const AxesControllerCreator& axes_controller_ctor);
		// void set_axes_properties(const manager::AxesProperties axes_properties);
		// void set_ctrlr_cfg_to_json(const AxesConfigToJsonTransformer& ctrlr_cfg_to_json);
		// void set_json_cfg_to_ctrlr(const JsonToAxesConfigTransformer& json_cfg_to_ctrlr);
	private:
		RawData *m_buffer;
		std::optional<RawDataPackageDescriptor> m_descriptor;
		std::optional<RawDataReaderInstance> m_raw_data_reader;
		std::optional<RawDataWriterInstance> m_raw_data_writer;
		std::optional<PackageSizeParser> m_size_parser;
		// std::optional<manager::AxesProperties> m_axes_properties;
		// std::optional<AxesConfigToJsonTransformer> m_ctrlr_cfg_to_json;
		// std::optional<JsonToAxesConfigTransformer> m_json_cfg_to_ctrlr;

		template <typename T>
		static const T& retrieve_from_option(const std::optional<T>& option, const std::string& option_name);
	};
}

#endif // MOVEMENT_HOST_BUILDER_HPP