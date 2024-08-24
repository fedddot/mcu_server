#ifndef	JSON_DATA_SERIALIZER_HPP
#define	JSON_DATA_SERIALIZER_HPP

#include <string>

#include "data.hpp"
#include "serializer.hpp"

namespace Json {
	class Value;
}

namespace server_utl {
	class JsonDataSerializer: public server::Serializer<std::string(const server::Data&)> {
	public:
		JsonDataSerializer() = default;
		JsonDataSerializer(const JsonDataSerializer& other) = default;
		JsonDataSerializer& operator=(const JsonDataSerializer& other) = default;

		std::string serialize(const server::Data& data) const override;
		server::Serializer<std::string(const server::Data&)> *clone() const override;
	private:
		static Json::Value parseJsonValue(const server::Data& obj);
		static Json::Value parseJsonObject(const server::Data& obj);
		static Json::Value parseJsonArray(const server::Data& obj);
		static Json::Value parseJsonString(const server::Data& obj);
		static Json::Value parseJsonInteger(const server::Data& obj);
	};

	inline server::Serializer<std::string(const server::Data&)> *JsonDataSerializer::clone() const {
		return new JsonDataSerializer(*this);
	}
}

#endif // JSON_DATA_SERIALIZER_HPP