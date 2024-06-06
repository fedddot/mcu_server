#ifndef	JSON_DATA_SERIALIZER_HPP
#define	JSON_DATA_SERIALIZER_HPP

#include <string>
#include "object.hpp"
#include "serializer.hpp"

namespace Json {
	class Value;
}

namespace server {
	class JsonDataSerializer: public Serializer<std::string(const Object&)> {
	public:
		JsonDataSerializer() = default;
		JsonDataSerializer(const JsonDataSerializer& other) = default;
		JsonDataSerializer& operator=(const JsonDataSerializer& other) = default;

		virtual std::string serialize(const Object& data) const override;
		Serializer<std::string(const Object&)> *clone() const override;
	private:
		static Json::Value parseJsonValueFromObject(const Object& obj);
	};

	inline Serializer<std::string(const Object&)> *JsonDataSerializer::clone() const {
		return new JsonDataSerializer(*this);
	}
}

#endif // JSON_DATA_SERIALIZER_HPP