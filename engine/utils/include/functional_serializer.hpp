#ifndef	FUNCTIONAL_SERIALIZER_HPP
#define	FUNCTIONAL_SERIALIZER_HPP

#include <functional>
#include <utility>

#include "serializer.hpp"

namespace engine_utl {
	template <typename Signature>
	class FunctionalSerializer;
	
	template <typename Tserial, typename... Args>
	class FunctionalSerializer<Tserial(Args...)>: public engine::Serializer<Tserial(Args...)> {
	public:
		FunctionalSerializer(const std::function<Tserial(Args...)>& action);
		FunctionalSerializer(const FunctionalSerializer& other) = default;
		FunctionalSerializer& operator=(const FunctionalSerializer& other) = default;

		Tserial serialize(Args... args) const override;
		engine::Serializer<Tserial(Args...)> *clone() const override;
	private:
		std::function<Tserial(Args...)> m_action;
	};

	template <typename Tserial, typename... Args>
	inline FunctionalSerializer<Tserial(Args...)>::FunctionalSerializer(const std::function<Tserial(Args...)>& action): m_action(action) {

	}

	template <typename Tserial, typename... Args>
	inline Tserial FunctionalSerializer<Tserial(Args...)>::serialize(Args... args) const {
		return m_action(std::forward<Args>(args)...);
	}

	template <typename Tserial, typename... Args>
	inline engine::Serializer<Tserial(Args...)> *FunctionalSerializer<Tserial(Args...)>::clone() const {
		return new FunctionalSerializer(*this);
	}
}
#endif // FUNCTIONAL_SERIALIZER_HPP