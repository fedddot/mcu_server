#ifndef	JSON_DATA_SERIALIZER_HPP
#define	JSON_DATA_SERIALIZER_HPP

#include <string>
#include "object.hpp"
#include "serializer.hpp"

namespace Json {
	class Value;
}

namespace server_utl {
	class JsonDataSerializer: public server::Serializer<std::string(const server::Object&)> {
	public:
		JsonDataSerializer() = default;
		JsonDataSerializer(const JsonDataSerializer& other) = default;
		JsonDataSerializer& operator=(const JsonDataSerializer& other) = default;

		virtual std::string serialize(const server::Object& data) const override;
		server::Serializer<std::string(const server::Object&)> *clone() const override;
	private:
		static Json::Value parseJsonValueFromObject(const server::Object& obj);
	};

	inline server::Serializer<std::string(const server::Object&)> *JsonDataSerializer::clone() const {
		return new JsonDataSerializer(*this);
	}
}

#endif // JSON_DATA_SERIALIZER_HPP