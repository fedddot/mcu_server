#ifndef	JSON_DATA_SERIALIZER_HPP
#define	JSON_DATA_SERIALIZER_HPP

#include <string>

#include "data.hpp"

namespace Json {
	class Value;
}

namespace server_utl {
	class JsonDataSerializer {
	public:
		JsonDataSerializer() = default;
		JsonDataSerializer(const JsonDataSerializer& other) = default;
		JsonDataSerializer& operator=(const JsonDataSerializer& other) = default;
		virtual ~JsonDataSerializer() noexcept = default;

		std::string operator()(const server::Data& data) const;
	private:
		static Json::Value parseJsonValue(const server::Data& obj);
		static Json::Value parseJsonObject(const server::Data& obj);
		static Json::Value parseJsonArray(const server::Data& obj);
		static Json::Value parseJsonString(const server::Data& obj);
		static Json::Value parseJsonInteger(const server::Data& obj);
	};
}

#endif // JSON_DATA_SERIALIZER_HPP