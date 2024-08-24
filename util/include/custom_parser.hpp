#ifndef	CUSTOM_PARSER_HPP
#define	CUSTOM_PARSER_HPP

#include <stdexcept>
#include <functional>

#include "parser.hpp"

namespace server_utl {
	template <typename Signature>
	class CustomParser;

	template <typename Tparsed, typename... Args>
	class CustomParser<Tparsed(Args...)>: public server::Parser<Tparsed(Args...)> {
	public:
		using CreateFunction = std::function<Tparsed(Args...)>;
		
		CustomParser(const CreateFunction& parse_function);
		CustomParser(const CustomParser& other) = default;
		CustomParser& operator=(const CustomParser& other) = default;
				
		Tparsed parse(Args... args) const override;
		server::Parser<Tparsed(Args...)> *clone() const override;
	private:
		CreateFunction m_parse_function;
	};

	template <typename Tparsed, typename... Args>
	inline CustomParser<Tparsed(Args...)>::CustomParser(const CreateFunction& parse_function): m_parse_function(parse_function) {
		if (!m_parse_function) {
			throw std::invalid_argument("invalid parse action received");
		}
	}

	template <typename Tparsed, typename... Args>
	inline Tparsed CustomParser<Tparsed(Args...)>::parse(Args... args) const {
		return m_parse_function(std::forward<Args>(args)...);
	}

	template <typename Tparsed, typename... Args>
	inline server::Parser<Tparsed(Args...)> *CustomParser<Tparsed(Args...)>::clone() const {
		return new CustomParser(*this);
	}
}

#endif // CUSTOM_PARSER_HPP