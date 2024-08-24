#ifndef	JSON_DATA_PARSER_HPP
#define	JSON_DATA_PARSER_HPP

#include <string>

#include "data.hpp"
#include "parser.hpp"

namespace Json {
	class Value;
}

namespace server_utl {
	class JsonDataParser: public server::Parser<server::Data *(const std::string&)> {
	public:
		JsonDataParser() = default;
		JsonDataParser(const JsonDataParser& other) = default;
		JsonDataParser& operator=(const JsonDataParser& other) = default;
		
		server::Data *parse(const std::string& data) const override;
		server::Parser<server::Data *(const std::string&)> *clone() const override;
	private:
		static server::Data *parseValue(const Json::Value& root);
		static server::Data *parseObject(const Json::Value& root);
		static server::Data *parseArray(const Json::Value& root);
		static server::Data *parseString(const Json::Value& root);
		static server::Data *parseInteger(const Json::Value& root);
	};

	inline server::Parser<server::Data *(const std::string&)> *JsonDataParser::clone() const {
		return new JsonDataParser(*this);
	}
}

#endif // JSON_DATA_PARSER_HPP