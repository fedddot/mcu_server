#ifndef	API_REQUEST_READER_BUILDER_HPP
#define	API_REQUEST_READER_BUILDER_HPP

#include <functional>
#include <optional>
#include <stdexcept>

#include "ipc_data_reader.hpp"
#include "ipc_instance.hpp"
#include "ipc_reader_builder.hpp"

namespace ipc {
	template <typename ApiRequest, typename RawData>
	class ApiRequestReaderBuilder: public IpcReaderBuilder<ApiRequest> {
	public:
		using ApiRequestParser = std::function<Instance<ApiRequest>(const RawData&)>;

		ApiRequestReaderBuilder() = default;
		ApiRequestReaderBuilder(const ApiRequestReaderBuilder&) = default;
		ApiRequestReaderBuilder& operator=(const ApiRequestReaderBuilder&) = default;
		Instance<IpcDataReader<ApiRequest>> build() const override;
		ApiRequestReaderBuilder& set_api_request_parser(const ApiRequestParser& api_request_parser);
		ApiRequestReaderBuilder& set_raw_data_reader(const Instance<IpcDataReader<RawData>>& reader);
	private:
		std::optional<ApiRequestParser> m_api_request_parser;
		std::optional<Instance<IpcDataReader<RawData>>> m_raw_data_reader;

		class ApiRequestReader: public IpcDataReader<ApiRequest> {
		public:
			ApiRequestReader(
				const ApiRequestParser& api_request_parser,
				const Instance<IpcDataReader<RawData>>& raw_data_reader
			);
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
		if (!m_api_request_parser || !m_raw_data_reader) {
			throw std::runtime_error("Builder is not properly initialized: missing required components.");
		}
		return Instance<IpcDataReader<ApiRequest>>(new ApiRequestReader(*m_api_request_parser, *m_raw_data_reader));
	}

	template <typename ApiRequest, typename RawData>
	inline ApiRequestReaderBuilder<ApiRequest, RawData>& ApiRequestReaderBuilder<ApiRequest, RawData>::set_api_request_parser(const ApiRequestParser& api_request_parser) {
		m_api_request_parser = api_request_parser;
		return std::ref(*this);
	}
	
	template <typename ApiRequest, typename RawData>
	inline ApiRequestReaderBuilder<ApiRequest, RawData>& ApiRequestReaderBuilder<ApiRequest, RawData>::set_raw_data_reader(const Instance<IpcDataReader<RawData>>& reader) {
		m_raw_data_reader = reader;
		return std::ref(*this);
	}

	template <typename ApiRequest, typename RawData>
	inline ApiRequestReaderBuilder<ApiRequest, RawData>::ApiRequestReader::ApiRequestReader(
		const ApiRequestParser& api_request_parser,
		const Instance<IpcDataReader<RawData>>& raw_data_reader
	): m_api_request_parser(api_request_parser), m_raw_data_reader(raw_data_reader) {
		if (!m_api_request_parser) {
			throw std::invalid_argument("ApiRequestParser is invalid: a valid parser must be provided.");
		}
	}

	template <typename ApiRequest, typename RawData>
	inline std::optional<Instance<ApiRequest>> ApiRequestReaderBuilder<ApiRequest, RawData>::ApiRequestReader::read() {
	    const auto raw_data_opt = m_raw_data_reader.get().read();
	    if (!raw_data_opt) {
	        return std::nullopt;
	    }
	    return m_api_request_parser(raw_data_opt->get());
	}
}

#endif // API_REQUEST_READER_BUILDER_HPP