#ifndef	JSON_DATA_PARSER_HPP
#define	JSON_DATA_PARSER_HPP

#include <string>
#include "object.hpp"
#include "parser.hpp"

namespace Json {
	class Value;
}

namespace server_utl {
	class JsonDataParser: public server::Parser<server::Object(const std::string&)> {
	public:
		JsonDataParser() = default;
		JsonDataParser(const JsonDataParser& other) = default;
		JsonDataParser& operator=(const JsonDataParser& other) = default;
		
		virtual server::Object parse(const std::string& data) const override;
		virtual server::Parser<server::Object(const std::string&)> *clone() const override;
	private:
		static server::Object parseObjectFromJsonValue(const Json::Value& root);
	};

	inline server::Parser<server::Object(const std::string&)> *JsonDataParser::clone() const {
		return new JsonDataParser(*this);
	}
}

#endif // JSON_DATA_PARSER_HPP