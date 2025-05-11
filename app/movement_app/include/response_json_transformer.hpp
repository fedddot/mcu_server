#ifndef	RESPONSE_JSON_TRANSFORMER_HPP
#define	RESPONSE_JSON_TRANSFORMER_HPP

#include "json/value.h"

#include "movement_manager_response.hpp"

namespace ipc {
	class ResponseJsonTransformer {
	public:
		ResponseJsonTransformer() = default;
		ResponseJsonTransformer(const ResponseJsonTransformer&) = default;
		ResponseJsonTransformer& operator=(const ResponseJsonTransformer&) = default;
		virtual ~ResponseJsonTransformer() noexcept = default;

		Json::Value response_to_json_value(const manager::MovementManagerResponse& response) const;
		manager::MovementManagerResponse json_value_to_response(const Json::Value& json_response) const;
	};

	inline Json::Value ResponseJsonTransformer::response_to_json_value(const manager::MovementManagerResponse& response) const {
		Json::Value json_data;
		json_data["result"] = static_cast<int>(response.code);
		if (response.message) {
			json_data["message"] = response.message.value();
		}
		return json_data;
	}

	inline manager::MovementManagerResponse ResponseJsonTransformer::json_value_to_response(const Json::Value& json_response) const {
		manager::MovementManagerResponse response;
		if (!json_response.isMember("result")) {
			throw std::runtime_error("invalid JSON response: missing 'result' field");
		}
		response.code = static_cast<manager::MovementManagerResponse::ResultCode>(json_response["result"].asInt());
		if (json_response.isMember("message")) {
			response.message = json_response["message"].asString();
		}
		return response;
	}
}

#endif // RESPONSE_JSON_TRANSFORMER_HPP