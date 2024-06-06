#ifndef	SERVER_SERIALIZER_HPP
#define	SERVER_SERIALIZER_HPP

#include <functional>
#include <utility>

#include "serializer.hpp"

namespace server_utl {
	template <typename Signature>
	class ServerSerializer;
	
	template <typename Tserial, typename... Args>
	class ServerSerializer<Tserial(Args...)>: public server::Serializer<Tserial(Args...)> {
	public:
		ServerSerializer(const std::function<Tserial(Args...)>& action);
		ServerSerializer(const ServerSerializer& other) = default;
		ServerSerializer& operator=(const ServerSerializer& other) = default;

		Tserial serialize(Args... args) const override;
		server::Serializer<Tserial(Args...)> *clone() const override;
	private:
		std::function<Tserial(Args...)> m_action;
	};

	template <typename Tserial, typename... Args>
	inline ServerSerializer<Tserial(Args...)>::ServerSerializer(const std::function<Tserial(Args...)>& action): m_action(action) {

	}

	template <typename Tserial, typename... Args>
	inline Tserial ServerSerializer<Tserial(Args...)>::serialize(Args... args) const {
		return m_action(std::forward<Args>(args)...);
	}

	template <typename Tserial, typename... Args>
	inline server::Serializer<Tserial(Args...)> *ServerSerializer<Tserial(Args...)>::clone() const {
		return new ServerSerializer(*this);
	}
}
#endif // SERVER_SERIALIZER_HPP