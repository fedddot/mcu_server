#ifndef	SERIALIZER_HPP
#define	SERIALIZER_HPP

namespace mcu_server {
	template <typename Signature>
	class Serializer;
	
	template <typename Tserial, typename... Args>
	class Serializer<Tserial(Args...)> {
	public:
		virtual ~Serializer() noexcept = default;
		virtual Tserial serialize(Args... args) const = 0;
		virtual Serializer *clone() const = 0;
	};
}

#endif // SERIALIZER_HPP