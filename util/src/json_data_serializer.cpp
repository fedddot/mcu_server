#include <stdexcept>
#include <string>

#include "array.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "string.hpp"

#include "json/writer.h"
#include "json/value.h"

#include "json_data_serializer.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;

std::string JsonDataSerializer::serialize(const Data& data) const {
	Json::StreamWriterBuilder wbuilder;
	wbuilder["indentation"] = "\t";
	return Json::writeString(wbuilder, parseJsonValue(data));	
}

Json::Value JsonDataSerializer::parseJsonValue(const Data& obj) {
	switch (obj.type()) {
	case Data::Type::OBJECT:
		return parseJsonObject(obj);
	case Data::Type::ARRAY:
		return parseJsonArray(obj);
	case Data::Type::STR:
		return parseJsonString(obj);
	case Data::Type::INT:
		return parseJsonInteger(obj);
	default:
		throw std::invalid_argument("unsupported data type received");
	}
}

Json::Value JsonDataSerializer::parseJsonObject(const Data& obj) {
	Json::Value root(Json::ValueType::objectValue);
	Data::cast<Object>(obj).for_each(
		[&root](const std::string& field_name, const Data& data) {
			root[field_name] = parseJsonValue(data);
		}
	);
	return root;
}

Json::Value JsonDataSerializer::parseJsonArray(const Data& obj) {
	Json::Value root(Json::ValueType::arrayValue);
	Data::cast<Array>(obj).for_each(
		[&root](int index, const Data& data) {
			root.append(parseJsonValue(data));
		}
	);
	return root;
}

Json::Value JsonDataSerializer::parseJsonString(const Data& obj) {
	return Json::Value(Data::cast<String>(obj).get());
}

Json::Value JsonDataSerializer::parseJsonInteger(const Data& obj) {
	return Json::Value(Data::cast<Integer>(obj).get());
}