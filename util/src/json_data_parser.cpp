#include <memory>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "double.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "array.hpp"
#include "string.hpp"

#include "json/reader.h"
#include "json/value.h"

#include "json_data_parser.hpp"

using namespace server;
using namespace server_utl;

Data *JsonDataParser::operator()(const std::string& data) const {
	Json::Value root;
	Json::Reader reader;
    reader.parse(data, root);
	return parseValue(root);
}

Data *JsonDataParser::parseValue(const Json::Value& root) {
	switch (root.type()) {
	case Json::ValueType::objectValue:
		return parseObject(root);
	case Json::ValueType::arrayValue:
		return parseArray(root);
	case Json::ValueType::intValue:
		return parseInteger(root);
	case Json::ValueType::stringValue:
		return parseString(root);
	case Json::ValueType::realValue:
		return parseDouble(root);
	default:
		throw std::invalid_argument("unsupported Json data type");
	}
}

Data *JsonDataParser::parseObject(const Json::Value& root) {
	Object result;
	for (auto member_name: root.getMemberNames()) {
		std::unique_ptr<Data> member_data(parseValue(root[member_name]));
		result.add(member_name, *member_data);
	}
	return result.clone();
}

Data *JsonDataParser::parseArray(const Json::Value& root) {
	Array result;
	for (int i = 0; i < root.size(); ++i) {
		std::unique_ptr<Data> member_data(parseValue(root[i]));
		result.push_back(*member_data);
	}
	return result.clone();
}

Data *JsonDataParser::parseString(const Json::Value& root) {
	return new String(root.asString());
}

Data *JsonDataParser::parseInteger(const Json::Value& root) {
	return new Integer(root.asInt());
}

Data *JsonDataParser::parseDouble(const Json::Value& root) {
	return new Double(root.asDouble());
}