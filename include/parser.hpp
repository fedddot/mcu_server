#ifndef	PARSER_HPP
#define	PARSER_HPP

namespace server {
	template <typename Signature>
	class Parser;
	
	template <typename Tparsed, typename... Args>
	class Parser<Tparsed(Args...)> {
	public:
		virtual ~Parser() noexcept = default;
		virtual Tparsed parse(Args... args) const = 0;
		virtual Parser *clone() const = 0;
	};
}

#endif // PARSER_HPP