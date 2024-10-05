#ifndef	JSON_DATA_PARSER_HPP
#define	JSON_DATA_PARSER_HPP

#include <string>

#include "data.hpp"
#include "parser.hpp"

namespace Json {
	class Value;
}

namespace mcu_server_utl {
	class JsonDataParser: public mcu_server::Parser<mcu_server::Data *(const std::string&)> {
	public:
		JsonDataParser() = default;
		JsonDataParser(const JsonDataParser& other) = default;
		JsonDataParser& operator=(const JsonDataParser& other) = default;
		
		mcu_server::Data *parse(const std::string& data) const override;
		mcu_server::Parser<mcu_server::Data *(const std::string&)> *clone() const override;
	private:
		static mcu_server::Data *parseValue(const Json::Value& root);
		static mcu_server::Data *parseObject(const Json::Value& root);
		static mcu_server::Data *parseArray(const Json::Value& root);
		static mcu_server::Data *parseString(const Json::Value& root);
		static mcu_server::Data *parseInteger(const Json::Value& root);
	};

	inline mcu_server::Parser<mcu_server::Data *(const std::string&)> *JsonDataParser::clone() const {
		return new JsonDataParser(*this);
	}
}

#endif // JSON_DATA_PARSER_HPP