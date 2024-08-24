#ifndef	TRANSFORMER_HPP
#define	TRANSFORMER_HPP

namespace mcu_server {
	template <typename Signature>
	class Transformer;
	
	template <typename Transformed, typename... Args>
	class Transformer<Transformed(Args...)> {
	public:
		virtual ~Transformer() noexcept = default;
		virtual Transformed transform(Args... args) const = 0;
		virtual Transformer *clone() const = 0;
	};
}

#endif // TRANSFORMER_HPP