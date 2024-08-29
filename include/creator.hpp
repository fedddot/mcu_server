#ifndef	CREATOR_HPP
#define	CREATOR_HPP

namespace server {
	template <typename Signature>
	class Creator;

	template <typename Tprod, typename ...Args>
	class Creator<Tprod(Args...)> {
	public:
		virtual ~Creator() noexcept = default;
		virtual Tprod create(Args...) const = 0;
		virtual Creator *clone() const = 0;
	};
}

#endif // CREATOR_HPP