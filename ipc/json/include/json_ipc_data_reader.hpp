#ifndef	JSON_REQUEST_READER_HPP
#define	JSON_REQUEST_READER_HPP

#include <functional>
#include <optional>
#include <stdexcept>
#include <vector>

#include "json/reader.h"
#include "json/value.h"

#include "ipc_data_reader.hpp"

namespace ipc {
	template <typename Request>
	class JsonIpcDataReader: public IpcDataReader<Request> {
	public:
		using RequestParser = std::function<Request(const Json::Value&)>;
		JsonIpcDataReader(
			const SharedRequestReader<std::vector<char>>& raw_data_reader,
			const RequestParser& request_parser
		);
		JsonIpcDataReader(const JsonIpcDataReader&) = delete;
		JsonIpcDataReader& operator=(const JsonIpcDataReader&) = delete;
		std::optional<Request> read() override;
	private:
		SharedRequestReader<std::vector<char>> m_raw_data_reader;
		const RequestParser m_request_parser;

		static Json::Value parse_raw_data(const std::vector<char>& data);
	};

	template <typename Request>
	inline JsonIpcDataReader<Request>::JsonIpcDataReader(
		const SharedRequestReader<std::vector<char>>& raw_data_reader,
		const RequestParser& request_parser
	): m_raw_data_reader(raw_data_reader), m_request_parser(request_parser) {
		if (!m_request_parser) {
			throw std::invalid_argument("invalid request parser received");
		}
	}

	template <typename Request>
	inline std::optional<Request> JsonIpcDataReader<Request>::read() {
		const auto raw_data = m_raw_data_reader.read();
		if (!raw_data) {
			return std::optional<Request>();
		}
		const auto json_data = parse_raw_data(*raw_data);
		const auto parsed_request = m_request_parser(json_data);
		return std::optional<Request>(parsed_request);
	}

	template <typename Request>
	Json::Value JsonIpcDataReader<Request>::parse_raw_data(const std::vector<char>& data) {
		Json::Value root;
	    Json::Reader reader;
		if (!reader.parse(std::string(data.begin(), data.end()), std::ref(root), true)) {
			throw std::runtime_error("failed to parse raw request data: " + reader.getFormattedErrorMessages());
		}
		return root;
	}
}

#endif // JSON_REQUEST_READER_HPP