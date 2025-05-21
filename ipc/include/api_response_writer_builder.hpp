#ifndef	API_RESPONSE_WRITER_BUILDER_HPP
#define	API_RESPONSE_WRITER_BUILDER_HPP

#include <functional>
#include <optional>
#include <stdexcept>

#include "ipc_data_writer.hpp"
#include "ipc_instance.hpp"
#include "ipc_writer_builder.hpp"

namespace ipc {
	template <typename ApiResponse, typename RawData>
	class ApiResponseWriterBuilder: public IpcWriterBuilder<ApiResponse> {
	public:
		using ApiResponseSerializer = std::function<RawData(const ApiResponse&)>;

		ApiResponseWriterBuilder() = default;
		ApiResponseWriterBuilder(const ApiResponseWriterBuilder&) = default;
		ApiResponseWriterBuilder& operator=(const ApiResponseWriterBuilder&) = default;
		Instance<IpcDataWriter<ApiResponse>> build() const override;
		ApiResponseWriterBuilder& set_api_response_serializer(const ApiResponseSerializer& api_response_serializer);
		ApiResponseWriterBuilder& set_raw_data_writer(const Instance<IpcDataWriter<RawData>>& writer);
	private:
		std::optional<ApiResponseSerializer> m_api_response_serializer;
		std::optional<Instance<IpcDataWriter<RawData>>> m_raw_data_writer;

		class ApiResponseWriter: public IpcDataWriter<ApiResponse> {
		public:
			ApiResponseWriter(
				const ApiResponseSerializer& api_response_serializer,
				const Instance<IpcDataWriter<RawData>>& raw_data_writer
			);
			ApiResponseWriter(const ApiResponseWriter&) = delete;
			ApiResponseWriter& operator=(const ApiResponseWriter&) = delete;
			void write(const ApiResponse& data) const override;
		private:
			ApiResponseSerializer m_api_response_serializer;
			Instance<IpcDataWriter<RawData>> m_raw_data_writer;
		};

		template <typename T>
		static const T& retrieve_from_option(const std::optional<T>& option, const std::string& option_name);
	};

	template <typename ApiResponse, typename RawData>
	inline Instance<IpcDataWriter<ApiResponse>> ApiResponseWriterBuilder<ApiResponse, RawData>::build() const {
		return Instance<IpcDataWriter<ApiResponse>>(
			new ApiResponseWriter(
				retrieve_from_option(m_api_response_serializer, "api response serializer"),
				retrieve_from_option(m_raw_data_writer, "raw data writer")
			)
		);
	}

	template <typename ApiResponse, typename RawData>
	inline ApiResponseWriterBuilder<ApiResponse, RawData>& ApiResponseWriterBuilder<ApiResponse, RawData>::set_api_response_serializer(const ApiResponseSerializer& api_response_serializer) {
		m_api_response_serializer = api_response_serializer;
		return std::ref(*this);
	}
	
	template <typename ApiResponse, typename RawData>
	inline ApiResponseWriterBuilder<ApiResponse, RawData>& ApiResponseWriterBuilder<ApiResponse, RawData>::set_raw_data_writer(const Instance<IpcDataWriter<RawData>>& writer) {
		m_raw_data_writer = writer;
		return std::ref(*this);
	}

	template <typename ApiResponse, typename RawData>
	inline ApiResponseWriterBuilder<ApiResponse, RawData>::ApiResponseWriter::ApiResponseWriter(
		const ApiResponseSerializer& api_response_serializer,
		const Instance<IpcDataWriter<RawData>>& raw_data_writer
	): m_api_response_serializer(api_response_serializer), m_raw_data_writer(raw_data_writer) {
		if (!m_api_response_serializer) {
			throw std::invalid_argument("ApiResponseSerializer is invalid: a valid serializer must be provided.");
		}
	}

	template <typename ApiResponse, typename RawData>
	inline void ApiResponseWriterBuilder<ApiResponse, RawData>::ApiResponseWriter::write(const ApiResponse& data) const {
		const auto serialized_response = m_api_response_serializer(data);
		m_raw_data_writer.get().write(serialized_response);
	}

	template <typename ApiResponse, typename RawData>
	template <typename T>
	inline const T& ApiResponseWriterBuilder<ApiResponse, RawData>::retrieve_from_option(const std::optional<T>& option, const std::string& option_name) {
		if (!option) {
			throw std::runtime_error(option_name + " has not been set");
		}
		return std::ref(*option);
	}
}

#endif // API_RESPONSE_WRITER_BUILDER_HPP