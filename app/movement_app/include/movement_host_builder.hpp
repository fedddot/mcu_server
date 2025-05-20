#ifndef	MOVEMENT_HOST_BUILDER_HPP
#define	MOVEMENT_HOST_BUILDER_HPP

#include <optional>
#include <stdexcept>

#include "api_request_reader_builder.hpp"
#include "host.hpp"
#include "ipc_data.hpp"
#include "ipc_data_reader.hpp"
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
		
		MovementHostBuilder(): m_buffer(nullptr) {
			
		}
		MovementHostBuilder(const MovementHostBuilder&) = default;
		MovementHostBuilder& operator=(const MovementHostBuilder&) = default;
		virtual ~MovementHostBuilder() noexcept = default;
		
		Host<vendor::MovementVendorApiRequest, vendor::MovementVendorApiResponse> build() const {
			if (!m_buffer) {
				throw std::runtime_error("raw data buffer ptr has not been set");
			}
			const auto package_reader = ipc::Instance<ipc::IpcDataReader<RawData>>(
				new ipc::RawDataPackageReader(
					m_buffer,
					retrieve_from_option(m_descriptor, "package descriptor"),
					retrieve_from_option(m_size_parser, "package size parser")
				)
			);
			ipc::ApiRequestReaderBuilder<vendor::MovementVendorApiRequest, RawData> api_request_reader_builder;
			api_request_reader_builder
				.set_raw_data_reader(package_reader);

			const auto api_request_reader = api_request_reader_builder.build();

			throw std::runtime_error("NOT IMPLEMENTED");
		}
		MovementHostBuilder& set_raw_data_buffer_ptr(RawData *buffer) {
			if (!buffer) {
				throw std::invalid_argument("invalid buffer ptr received");
			}
			m_buffer = buffer;
			return std::ref(*this);
		}
		MovementHostBuilder& set_package_descriptor(const RawDataPackageDescriptor& descriptor) {
			m_descriptor = descriptor;
			return std::ref(*this);
		}
		MovementHostBuilder& set_package_size_parser(const PackageSizeParser& size_parser) {
			if (!size_parser) {
				throw std::invalid_argument("invalid size parser received");
			}
			m_size_parser = size_parser;
			return std::ref(*this);
		}
	private:
		RawData *m_buffer;
		std::optional<RawDataPackageDescriptor> m_descriptor;
		std::optional<PackageSizeParser> m_size_parser;

		template <typename T>
		static const T& retrieve_from_option(const std::optional<T>& option, const std::string& option_name) {
			if (!option) {
				throw std::runtime_error(option_name + " has not been set");
			}
			return std::ref(*option);
		}
	};
}

#endif // MOVEMENT_HOST_BUILDER_HPP