#ifndef	MOVEMENT_HOST_BUILDER_HPP
#define	MOVEMENT_HOST_BUILDER_HPP

#include "host.hpp"
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
		
		Host<Request, Response> build() const;
	private:
		std::optional<ipc::Instance<RawDataReader>> m_raw_data_reader;
		std::optional<ipc::Instance<RawDataWriter>> m_raw_data_writer;
		std::optional<AxesControllerCreator> m_axes_controller_ctor;
		std::optional<manager::AxesProperties> m_axes_properties;
		std::optional<AxisControllerConfigToJsonTransformer> m_ctrlr_cfg_to_json;
		std::optional<JsonToAxisControllerConfigTransformer> m_json_cfg_to_ctrlr;

		template <typename T>
		static const T& retrieve_from_option(const std::optional<T>& option);
	};

	template <typename AxisControllerConfig>
	inline Host<typename MovementHostBuilder<AxisControllerConfig>::Request, typename MovementHostBuilder<AxisControllerConfig>::Response> MovementHostBuilder<AxisControllerConfig>::build() const {
		return Host(
			ipc::Instance<ipc::IpcDataReader<manager::MovementManagerRequest>>(
				new ipc::MovementIpcDataReader<AxisControllerConfig>(
					retrieve_from_option(m_raw_data_reader),
					retrieve_from_option(m_ctrlr_cfg_to_json),
					retrieve_from_option(m_json_cfg_to_ctrlr)
				)
			),
			ipc::Instance<ipc::IpcDataWriter<manager::MovementManagerResponse>>(
				new ipc::MovementIpcDataWriter(
					retrieve_from_option(m_raw_data_writer)
				)
			),
			manager::Instance<manager::Manager<manager::MovementManagerRequest, manager::MovementManagerResponse>>(
				new manager::MovementManager<AxisControllerConfig>(
					retrieve_from_option(m_axes_controller_ctor),
					retrieve_from_option(m_axes_properties)
				)
			),
			[](const std::exception& e) {
				return manager::MovementManagerResponse {
					manager::MovementManagerResponse::ResultCode::EXCEPTION,
					std::string(e.what()),
				};
			}
		)
	}
}

#endif // MOVEMENT_HOST_BUILDER_HPP