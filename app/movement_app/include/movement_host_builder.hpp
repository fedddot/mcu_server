#ifndef	MOVEMENT_HOST_BUILDER_HPP
#define	MOVEMENT_HOST_BUILDER_HPP

#include <optional>
#include <stdexcept>

#include "api_request_reader_builder.hpp"
#include "host.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"
#include "movement_vendor_api_request.hpp"
#include "movement_vendor_api_response.hpp"

namespace host {
	template <typename AxesConfig, typename RawData>
	class MovementHostBuilder {
	public:
		using ApiRequest = vendor::MovementVendorApiRequest;
		using ApiRequestParser = typename ipc::ApiRequestReaderBuilder<ApiRequest, RawData>::ApiRequestParser;
		using RawDataReaderInstance = ipc::Instance<ipc::IpcDataReader<RawData>>;
		
		MovementHostBuilder() = default;
		MovementHostBuilder(const MovementHostBuilder&) = default;
		MovementHostBuilder& operator=(const MovementHostBuilder&) = default;
		virtual ~MovementHostBuilder() noexcept = default;
		
		Host<vendor::MovementVendorApiRequest, vendor::MovementVendorApiResponse> build() const {
			const auto api_request_reader = m_api_request_reader_builder.build();
			
			throw std::runtime_error("NOT IMPLEMENTED");
		}
		MovementHostBuilder& set_api_request_parser(const ApiRequestParser& api_request_parser) {
			m_api_request_reader_builder.set_api_request_parser(api_request_parser);
			return std::ref(*this);
		}
		MovementHostBuilder& set_raw_data_reader(const RawDataReaderInstance& raw_data_reader) {
			m_api_request_reader_builder.set_raw_data_reader(raw_data_reader);
			return std::ref(*this);
		}
	private:
		ipc::ApiRequestReaderBuilder<vendor::MovementVendorApiRequest, RawData> m_api_request_reader_builder;

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