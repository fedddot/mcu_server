#ifndef	MOVEMENT_HOST_BUILDER_HPP
#define	MOVEMENT_HOST_BUILDER_HPP

#include "host.hpp"
#include "host_instance.hpp"
#include "ipc_instance.hpp"
#include "ipc_data_reader.hpp"
#include "ipc_data_writer.hpp"
#include "manager_instance.hpp"
#include "movement_ipc_data_reader.hpp"
#include "movement_ipc_data_writer.hpp"
#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include <optional>
#include <stdexcept>

namespace host {
	template <typename AxisControllerConfig>
	class MovementHostBuilder {
	public:
		using Request = manager::MovementManagerRequest;
		using Response = manager::MovementManagerResponse;
		using AxisControllerConfigToJsonTransformer = typename ipc::RequestJsonTransformer<AxisControllerConfig>::AxisControllerConfigToJsonTransformer;
		using JsonToAxisControllerConfigTransformer = typename ipc::RequestJsonTransformer<AxisControllerConfig>::JsonToAxisControllerConfigTransformer;
		using AxesControllerCreator = typename manager::MovementManager<AxisControllerConfig>::AxesControllerCreator;
		using RawDataReader = ipc::IpcDataReader<ipc::RawData>;
		using RawDataWriter = ipc::IpcDataWriter<ipc::RawData>;
		
		MovementHostBuilder() = default;
		MovementHostBuilder(const MovementHostBuilder&) = default;
		MovementHostBuilder& operator=(const MovementHostBuilder&) = default;
		virtual ~MovementHostBuilder() noexcept = default;
		
		host::Instance<Host<Request, Response>> build() const;
		void set_raw_data_reader(const ipc::Instance<RawDataReader>& raw_data_reader);
		void set_raw_data_writer(const ipc::Instance<RawDataWriter>& raw_data_writer);
		void set_axes_controller_ctor(const AxesControllerCreator& axes_controller_ctor);
		void set_axes_properties(const manager::AxesProperties axes_properties);
		void set_ctrlr_cfg_to_json(const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json);
		void set_json_cfg_to_ctrlr(const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr);
	private:
		std::optional<ipc::Instance<RawDataReader>> m_raw_data_reader;
		std::optional<ipc::Instance<RawDataWriter>> m_raw_data_writer;
		std::optional<AxesControllerCreator> m_axes_controller_ctor;
		std::optional<manager::AxesProperties> m_axes_properties;
		std::optional<AxisControllerConfigToJsonTransformer> m_ctrlr_cfg_to_json;
		std::optional<JsonToAxisControllerConfigTransformer> m_json_cfg_to_ctrlr;

		template <typename T>
		static const T& retrieve_from_option(const std::optional<T>& option, const std::string& option_name);
	};

	template <typename AxisControllerConfig>
	inline host::Instance<Host<typename MovementHostBuilder<AxisControllerConfig>::Request, typename MovementHostBuilder<AxisControllerConfig>::Response>> MovementHostBuilder<AxisControllerConfig>::build() const {
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

	template <typename AxisControllerConfig>
	inline void MovementHostBuilder<AxisControllerConfig>::set_axes_properties(const manager::AxesProperties axes_properties) {
		m_axes_properties = axes_properties;
	}

	template <typename AxisControllerConfig>
	inline void MovementHostBuilder<AxisControllerConfig>::set_raw_data_reader(const ipc::Instance<RawDataReader>& raw_data_reader) {
		m_raw_data_reader = raw_data_reader;
	}

	template <typename AxisControllerConfig>
	inline void MovementHostBuilder<AxisControllerConfig>::set_raw_data_writer(const ipc::Instance<RawDataWriter>& raw_data_writer) {
		m_raw_data_writer = raw_data_writer;
	}

	template <typename AxisControllerConfig>
	inline void MovementHostBuilder<AxisControllerConfig>::set_axes_controller_ctor(const AxesControllerCreator& axes_controller_ctor) {
		m_axes_controller_ctor = axes_controller_ctor;
	}

	template <typename AxisControllerConfig>
	inline void MovementHostBuilder<AxisControllerConfig>::set_ctrlr_cfg_to_json(const AxisControllerConfigToJsonTransformer& ctrlr_cfg_to_json) {
		m_ctrlr_cfg_to_json = ctrlr_cfg_to_json;
	}

	template <typename AxisControllerConfig>
	inline void MovementHostBuilder<AxisControllerConfig>::set_json_cfg_to_ctrlr(const JsonToAxisControllerConfigTransformer& json_cfg_to_ctrlr) {
		m_json_cfg_to_ctrlr = json_cfg_to_ctrlr;
	}

	template <typename AxisControllerConfig>
	template <typename T>
	inline const T& MovementHostBuilder<AxisControllerConfig>::retrieve_from_option(const std::optional<T>& option, const std::string& option_name) {
		if (!option) {
			throw std::runtime_error("builder option " + option_name + " is not initialized");
		}
		return std::ref(*option);
	}
}

#endif // MOVEMENT_HOST_BUILDER_HPP