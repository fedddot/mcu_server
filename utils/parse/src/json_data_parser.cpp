#include <stdexcept>
#include <string>

#include "integer.hpp"
#include "object.hpp"
#include "string.hpp"

#include "json/reader.h"
#include "json/value.h"

#include "json_data_parser.hpp"

using namespace engine;
using namespace mcu_control_utl;

Object JsonDataParser::parse(const std::string& data) const {
	Json::Value root;
	Json::Reader reader;
    reader.parse(data, root);
	if (Json::ValueType::objectValue != root.type()) {
		throw std::invalid_argument("received data doesn't contain a JSON object");
	}
	return parseObjectFromJsonValue(root);	
}

Object JsonDataParser::parseObjectFromJsonValue(const Json::Value& root) {
	Object result;
	for (auto member_name: root.getMemberNames()) {
		auto member = root[member_name];
		switch (member.type()) {
		case Json::ValueType::intValue:
			result.add(member_name, Integer(member.asInt()));
			break;
		case Json::ValueType::stringValue:
			result.add(member_name, String(member.asString()));
			break;
		case Json::ValueType::objectValue:
			result.add(member_name, parseObjectFromJsonValue(member));
			break;
		default:
			throw std::invalid_argument("failed to parse data object - unsupported data type");
		}
	}
	return result;
}