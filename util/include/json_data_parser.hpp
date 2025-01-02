#ifndef	JSON_DATA_PARSER_HPP
#define	JSON_DATA_PARSER_HPP

#include <string>

#include "data.hpp"

namespace Json {
	class Value;
}

namespace util {
	class JsonDataParser {
	public:
		JsonDataParser() = default;
		JsonDataParser(const JsonDataParser& other) = default;
		JsonDataParser& operator=(const JsonDataParser& other) = default;
		virtual ~JsonDataParser() noexcept = default;
		
		manager::Data *operator()(const std::string& data) const;
	private:
		static manager::Data *parseValue(const Json::Value& root);
		static manager::Data *parseObject(const Json::Value& root);
		static manager::Data *parseArray(const Json::Value& root);
		static manager::Data *parseString(const Json::Value& root);
		static manager::Data *parseInteger(const Json::Value& root);
		static manager::Data *parseDouble(const Json::Value& root);
	};
}

#endif // JSON_DATA_PARSER_HPP