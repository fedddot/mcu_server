#ifndef	JSON_DATA_PARSER_HPP
#define	JSON_DATA_PARSER_HPP

#include <string>

#include "data.hpp"

namespace Json {
	class Value;
}

namespace server_utl {
	class JsonDataParser {
	public:
		JsonDataParser() = default;
		JsonDataParser(const JsonDataParser& other) = default;
		JsonDataParser& operator=(const JsonDataParser& other) = default;
		virtual ~JsonDataParser() noexcept = default;
		
		server::Data *operator()(const std::string& data) const;
	private:
		static server::Data *parseValue(const Json::Value& root);
		static server::Data *parseObject(const Json::Value& root);
		static server::Data *parseArray(const Json::Value& root);
		static server::Data *parseString(const Json::Value& root);
		static server::Data *parseInteger(const Json::Value& root);
		static server::Data *parseDouble(const Json::Value& root);
	};
}

#endif // JSON_DATA_PARSER_HPP