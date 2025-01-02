#ifndef	JSON_DATA_SERIALIZER_HPP
#define	JSON_DATA_SERIALIZER_HPP

#include <string>

#include "data.hpp"

namespace Json {
	class Value;
}

namespace manager_utl {
	class JsonDataSerializer {
	public:
		JsonDataSerializer() = default;
		JsonDataSerializer(const JsonDataSerializer& other) = default;
		JsonDataSerializer& operator=(const JsonDataSerializer& other) = default;
		virtual ~JsonDataSerializer() noexcept = default;

		std::string operator()(const manager::Data& data) const;
	private:
		static Json::Value parseJsonValue(const manager::Data& obj);
		static Json::Value parseJsonObject(const manager::Data& obj);
		static Json::Value parseJsonArray(const manager::Data& obj);
		static Json::Value parseJsonString(const manager::Data& obj);
		static Json::Value parseJsonInteger(const manager::Data& obj);
		static Json::Value parseJsonDouble(const manager::Data& obj);
	};
}

#endif // JSON_DATA_SERIALIZER_HPP