#ifndef	SERVER_PARSER_HPP
#define	SERVER_PARSER_HPP

#include <functional>
#include <utility>

#include "parser.hpp"

namespace server_utl {
	template <typename Signature>
	class ServerParser;
	
	template <typename Tparsed, typename... Args>
	class ServerParser<Tparsed(Args...)>: public server::Parser<Tparsed(Args...)> {
	public:
		ServerParser(const std::function<Tparsed(Args...)>& action);
		ServerParser(const ServerParser& other) = default;
		ServerParser& operator=(const ServerParser& other) = default;

		Tparsed parse(Args... args) const override;
		server::Parser<Tparsed(Args...)> *clone() const override;
	private:
		std::function<Tparsed(Args...)> m_action;
	};

	template <typename Tparsed, typename... Args>
	inline ServerParser<Tparsed(Args...)>::ServerParser(const std::function<Tparsed(Args...)>& action): m_action(action) {

	}

	template <typename Tparsed, typename... Args>
	inline Tparsed ServerParser<Tparsed(Args...)>::parse(Args... args) const {
		return m_action(std::forward<Args>(args)...);
	}

	template <typename Tparsed, typename... Args>
	inline server::Parser<Tparsed(Args...)> *ServerParser<Tparsed(Args...)>::clone() const {
		return new ServerParser(*this);
	}
}
#endif // SERVER_PARSER_HPP