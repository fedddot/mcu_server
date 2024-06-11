#ifndef	FUNCTIONAL_PARSER_HPP
#define	FUNCTIONAL_PARSER_HPP

#include <functional>
#include <utility>

#include "parser.hpp"

namespace engine_utl {
	template <typename Signature>
	class FunctionalParser;
	
	template <typename Tparsed, typename... Args>
	class FunctionalParser<Tparsed(Args...)>: public engine::Parser<Tparsed(Args...)> {
	public:
		FunctionalParser(const std::function<Tparsed(Args...)>& action);
		FunctionalParser(const FunctionalParser& other) = default;
		FunctionalParser& operator=(const FunctionalParser& other) = default;

		Tparsed parse(Args... args) const override;
		engine::Parser<Tparsed(Args...)> *clone() const override;
	private:
		std::function<Tparsed(Args...)> m_action;
	};

	template <typename Tparsed, typename... Args>
	inline FunctionalParser<Tparsed(Args...)>::FunctionalParser(const std::function<Tparsed(Args...)>& action): m_action(action) {

	}

	template <typename Tparsed, typename... Args>
	inline Tparsed FunctionalParser<Tparsed(Args...)>::parse(Args... args) const {
		return m_action(std::forward<Args>(args)...);
	}

	template <typename Tparsed, typename... Args>
	inline engine::Parser<Tparsed(Args...)> *FunctionalParser<Tparsed(Args...)>::clone() const {
		return new FunctionalParser(*this);
	}
}
#endif // FUNCTIONAL_PARSER_HPP