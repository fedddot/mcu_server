#ifndef	API_REQUEST_READER_BUILDER_HPP
#define	API_REQUEST_READER_BUILDER_HPP

#include <functional>
#include <optional>

#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"

namespace ipc {
	template <typename ApiRequest, typename RawData>
	class ApiRequestReaderBuilder {
	public:
		using ApiRequestParser = std::function<Instance<ApiRequest>(const RawData&)>;

		ApiRequestReaderBuilder() = default;
		ApiRequestReaderBuilder(const ApiRequestReaderBuilder&) = default;
		ApiRequestReaderBuilder& operator=(const ApiRequestReaderBuilder&) = default;
		virtual ~ApiRequestReaderBuilder() noexcept = default;
		Instance<IpcDataReader<ApiRequest>> build() const;
		void set_api_request_parser(const ApiRequestParser& api_request_parser);
		void set_raw_data_reader(const Instance<IpcDataReader<RawData>>& reader);
	private:
		std::optional<ApiRequestParser> m_api_request_parser;
		std::optional<Instance<IpcDataReader<RawData>>> m_raw_data_reader;

		class ApiRequestReader: public IpcDataReader<ApiRequest> {
		public:
			ApiRequestReader(const ApiRequestReaderBuilder *builder_ptr);
			ApiRequestReader(const ApiRequestReader&) = delete;
			ApiRequestReader& operator=(const ApiRequestReader&) = delete;
			std::optional<Instance<ApiRequest>> read() override;
		private:
			ApiRequestParser m_api_request_parser;
			Instance<IpcDataReader<RawData>> m_raw_data_reader;
		};
	};

	template <typename ApiRequest, typename RawData>
	inline Instance<IpcDataReader<ApiRequest>> ApiRequestReaderBuilder<ApiRequest, RawData>::build() const {
		return Instance(new ApiRequestReader(this));
	}

	template <typename ApiRequest, typename RawData>
	inline void ApiRequestReaderBuilder<ApiRequest, RawData>::set_api_request_parser(const ApiRequestParser& api_request_parser) {
		m_api_request_parser = api_request_parser;
	}
	
	template <typename ApiRequest, typename RawData>
	inline void ApiRequestReaderBuilder<ApiRequest, RawData>::set_raw_data_reader(const Instance<IpcDataReader<RawData>>& reader) {
		m_raw_data_reader = reader;
	}
}

#endif // API_REQUEST_READER_BUILDER_HPP