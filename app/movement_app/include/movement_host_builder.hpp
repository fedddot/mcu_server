#ifndef	MOVEMENT_HOST_BUILDER_HPP
#define	MOVEMENT_HOST_BUILDER_HPP

#include <optional>
#include "api_request_reader_builder.hpp"
#include "host.hpp"
#include "ipc_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "movement_manager_request.hpp"
#include "movement_vendor_api_request.hpp"
#include "movement_vendor_manager_id.hpp"

namespace host {
	using ManagerId = vendor::MovementManagerId;
	using Payload = manager::MovementManagerRequest;

	template <typename AxisControllerConfig, typename RawData>
	class MovementHostBuilder {
	public:
		using RawDataReaderInstance = ipc::Instance<ipc::IpcDataReader<RawData>>;
		using RawDataWriterInstance = ipc::Instance<ipc::IpcDataWriter<RawData>>;
		
		MovementHostBuilder() = default;
		MovementHostBuilder(const MovementHostBuilder&) = default;
		MovementHostBuilder& operator=(const MovementHostBuilder&) = default;
		virtual ~MovementHostBuilder() noexcept = default;
		
		Host<ManagerId, Payload> build() const;

		void set_raw_data_reader(const RawDataReaderInstance& raw_data_reader);
		void set_raw_data_writer(const RawDataWriterInstance& raw_data_writer);
		// void set_axes_controller_ctor(const AxesControllerCreator& axes_controller_ctor);
		// void set_axes_properties(const manager::AxesProperties axes_properties);
		// void set_ctrlr_cfg_to_json(const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json);
		// void set_json_cfg_to_ctrlr(const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr);
	private:
		ipc::ApiRequestReaderBuilder<vendor::MovementVendorApiRequest, typename RawData>
		// std::optional<AxesControllerCreator> m_axes_controller_ctor;
		// std::optional<manager::AxesProperties> m_axes_properties;
		// std::optional<AxisControllerConfigToJsonTransformer> m_ctrlr_cfg_to_json;
		// std::optional<JsonToAxisControllerConfigTransformer> m_json_cfg_to_ctrlr;

		template <typename T>
		static const T& retrieve_from_option(const std::optional<T>& option, const std::string& option_name);
	};

	template <typename AxisControllerConfig, typename RawData>
	inline Host<ManagerId, Payload> MovementHostBuilder<AxisControllerConfig, RawData>::build() const {
		return host::Instance(
			new Host<Request, Response>(
				ipc::Instance<ipc::IpcDataReader<manager::MovementManagerRequest>>(
					new ipc::MovementIpcDataReader<AxisControllerConfig>(
						retrieve_from_option(m_raw_data_reader, "raw_data_reader"),
						retrieve_from_option(m_ctrlr_cfg_to_json, "ctrlr_cfg_to_json"),
						retrieve_from_option(m_json_cfg_to_ctrlr, "json_cfg_to_ctrlr")
					)
				),
				ipc::Instance<ipc::IpcDataWriter<manager::MovementManagerResponse>>(
					new ipc::MovementIpcDataWriter(
						retrieve_from_option(m_raw_data_writer, "raw_data_writer")
					)
				),
				manager::Instance<manager::Manager<manager::MovementManagerRequest, manager::MovementManagerResponse>>(
					new manager::MovementManager<AxisControllerConfig>(
						retrieve_from_option(m_axes_controller_ctor, "axes_controller_ctor"),
						retrieve_from_option(m_axes_properties, "axes_properties")
					)
				),
				[](const std::exception& e) {
					return manager::MovementManagerResponse {
						manager::MovementManagerResponse::ResultCode::EXCEPTION,
						std::string(e.what()),
					};
				}
			)
		);
	}

	template <typename AxisControllerConfig, typename RawData>
	inline void MovementHostBuilder<AxisControllerConfig, RawData>::set_axes_properties(const manager::AxesProperties axes_properties) {
		m_axes_properties = axes_properties;
	}

	template <typename AxisControllerConfig, typename RawData>
	inline void MovementHostBuilder<AxisControllerConfig, RawData>::set_raw_data_reader(const RawDataReaderInstance& raw_data_reader) {
		m_raw_data_reader = raw_data_reader;
	}

	template <typename AxisControllerConfig, typename RawData>
	inline void MovementHostBuilder<AxisControllerConfig, RawData>::set_raw_data_writer(const RawDataWriterInstance& raw_data_writer) {
		m_raw_data_writer = raw_data_writer;
	}

	template <typename AxisControllerConfig, typename RawData>
	inline void MovementHostBuilder<AxisControllerConfig, RawData>::set_axes_controller_ctor(const AxesControllerCreator& axes_controller_ctor) {
		m_axes_controller_ctor = axes_controller_ctor;
	}

	template <typename AxisControllerConfig, typename RawData>
	inline void MovementHostBuilder<AxisControllerConfig, RawData>::set_ctrlr_cfg_to_json(const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json) {
		m_ctrlr_cfg_to_json = ctrlr_cfg_to_json;
	}

	template <typename AxisControllerConfig, typename RawData>
	inline void MovementHostBuilder<AxisControllerConfig, RawData>::set_json_cfg_to_ctrlr(const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr) {
		m_json_cfg_to_ctrlr = json_cfg_to_ctrlr;
	}

	template <typename AxisControllerConfig, typename RawData>
	template <typename T>
	inline const T& MovementHostBuilder<AxisControllerConfig, RawData>::retrieve_from_option(const std::optional<T>& option, const std::string& option_name) {
		if (!option) {
			throw std::runtime_error("builder option " + option_name + " is not initialized");
		}
		return std::ref(*option);
	}
}

#endif // MOVEMENT_HOST_BUILDER_HPP