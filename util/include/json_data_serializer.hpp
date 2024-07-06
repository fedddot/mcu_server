#ifndef	JSON_DATA_SERIALIZER_HPP
#define	JSON_DATA_SERIALIZER_HPP

#include <string>

#include "data.hpp"
#include "serializer.hpp"

namespace Json {
	class Value;
}

namespace mcu_server_utl {
	class JsonDataSerializer: public mcu_server::Serializer<std::string(const engine::Data&)> {
	public:
		JsonDataSerializer() = default;
		JsonDataSerializer(const JsonDataSerializer& other) = default;
		JsonDataSerializer& operator=(const JsonDataSerializer& other) = default;

		std::string serialize(const engine::Data& data) const override;
		mcu_server::Serializer<std::string(const engine::Data&)> *clone() const override;
	private:
		static Json::Value parseJsonValue(const engine::Data& obj);
		static Json::Value parseJsonObject(const engine::Data& obj);
		static Json::Value parseJsonString(const engine::Data& obj);
		static Json::Value parseJsonInteger(const engine::Data& obj);
	};

	inline mcu_server::Serializer<std::string(const engine::Data&)> *JsonDataSerializer::clone() const {
		return new JsonDataSerializer(*this);
	}
}

#endif // JSON_DATA_SERIALIZER_HPP