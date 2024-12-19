#ifndef	JSON_REQUEST_PARSER_HPP
#define	JSON_REQUEST_PARSER_HPP

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "json_data_parser.hpp"
#include "object.hpp"
#include "request.hpp"
#include "server_types.hpp"
#include "string.hpp"

namespace server_utl {
	class JsonRequestParser {
	public:
		JsonRequestParser() = default;
		JsonRequestParser(const JsonRequestParser& other) = default;
		JsonRequestParser& operator=(const JsonRequestParser& other) = default;
		virtual ~JsonRequestParser() noexcept = default;
		
		server::Request operator()(const std::string& data) const;
	private:
		static typename server::Request::Method parse_method(const server::Data& data);
		static typename server::Path parse_path(const server::Data& data);
		static typename server::Body parse_body(const server::Data& data);
	};

	inline server::Request JsonRequestParser::operator()(const std::string& data) const {
		using namespace server;
		std::unique_ptr<Data> parsed_data(JsonDataParser()(data));
		const auto& data_obj(Data::cast<Object>(*parsed_data));
		return Request(
			parse_method(data_obj.access("method")),
			parse_path(data_obj.access("path")),
			parse_body(data_obj.access("body"))
		);
	}

	inline typename server::Request::Method JsonRequestParser::parse_method(const server::Data& data) {
		using namespace server;
		auto data_str(Data::cast<String>(data).get());
		if ("CREATE" == data_str) {
			return Request::Method::CREATE;
		} else if ("READ" == data_str) {
			return Request::Method::READ;
		} else if ("UPDATE" == data_str) {
			return Request::Method::UPDATE;
		} else if ("DELETE" == data_str) {
			return Request::Method::DELETE;
		} else {
			throw std::invalid_argument("failed to parse request method");
		}
	}

	inline typename server::Path JsonRequestParser::parse_path(const server::Data& data) {
		using namespace server;
		const char delim('/');
		auto data_str(Data::cast<String>(data).get());
		Path path;
		auto start_iter(data_str.begin());
		while (true) {
			auto delim_iter(std::find(start_iter, data_str.end(), delim));
			path.push_back(std::string(start_iter, delim_iter));
			if (data_str.end() == delim_iter) {
				break;
			}
			start_iter = delim_iter + 1;
		}
		return path;
	}

	inline typename server::Body JsonRequestParser::parse_body(const server::Data& data) {
		using namespace server;
		return Data::cast<Object>(data);
	}
}

#endif // JSON_REQUEST_PARSER_HPP