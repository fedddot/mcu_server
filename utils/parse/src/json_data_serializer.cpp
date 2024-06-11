#include <stdexcept>
#include <string>

#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "string.hpp"

#include "json/writer.h"
#include "json/value.h"

#include "json_data_serializer.hpp"

using namespace engine;
using namespace server_utl;

std::string JsonDataSerializer::serialize(const Object& data) const {
	Json::StreamWriterBuilder wbuilder;
	wbuilder["indentation"] = "\t";
	return Json::writeString(wbuilder, parseJsonValueFromObject(data));	
}

Json::Value JsonDataSerializer::parseJsonValueFromObject(const Object& obj) {
	Json::Value root(Json::ValueType::objectValue);
	obj.for_each(
		[&root](const std::string& field_name, const Data& data) {
			switch (data.type()) {
			case Data::Type::INT:
				root[field_name] = Data::cast<Integer>(data).get();
				break;
			case Data::Type::STR:
				root[field_name] = Data::cast<String>(data).get();
				break;
			case Data::Type::OBJECT:
				root[field_name] = parseJsonValueFromObject(Data::cast<Object>(data));
				break;
			default:
				throw std::invalid_argument("unsupported data type received in parseJsonValueFromObject function");
			}
		}
	);
	return root;
}