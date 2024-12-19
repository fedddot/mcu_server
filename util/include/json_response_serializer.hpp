#ifndef	JSON_RESPONSE_SERIALIZER_HPP
#define	JSON_RESPONSE_SERIALIZER_HPP

#include <string>

#include "integer.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "response.hpp"

namespace server_utl {
	class JsonResponseSerializer {
	public:
		JsonResponseSerializer() = default;
		JsonResponseSerializer(const JsonResponseSerializer& other) = default;
		JsonResponseSerializer& operator=(const JsonResponseSerializer& other) = default;
		virtual ~JsonResponseSerializer() noexcept = default;
		
		std::string operator()(const server::Response& data) const;
	};

	inline std::string JsonResponseSerializer::operator()(const server::Response& data) const {
		using namespace server;
		Object response_data;
		response_data.add("code", Integer(static_cast<int>(data.code())));
		response_data.add("body", data.body());
		return JsonDataSerializer()(response_data);
	}
}

#endif // JSON_RESPONSE_SERIALIZER_HPP