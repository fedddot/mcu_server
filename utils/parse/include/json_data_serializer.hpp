#ifndef	JSON_DATA_SERIALIZER_HPP
#define	JSON_DATA_SERIALIZER_HPP

#include <string>
#include "object.hpp"
#include "serializer.hpp"

namespace Json {
	class Value;
}

namespace mcu_control_utl {
	class JsonDataSerializer: public engine::Serializer<std::string(const engine::Object&)> {
	public:
		JsonDataSerializer() = default;
		JsonDataSerializer(const JsonDataSerializer& other) = default;
		JsonDataSerializer& operator=(const JsonDataSerializer& other) = default;

		virtual std::string serialize(const engine::Object& data) const override;
		engine::Serializer<std::string(const engine::Object&)> *clone() const override;
	private:
		static Json::Value parseJsonValueFromObject(const engine::Object& obj);
	};

	inline engine::Serializer<std::string(const engine::Object&)> *JsonDataSerializer::clone() const {
		return new JsonDataSerializer(*this);
	}
}

#endif // JSON_DATA_SERIALIZER_HPP