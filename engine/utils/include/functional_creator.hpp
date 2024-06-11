#ifndef	FUNCTIONAL_CREATOR_HPP
#define	FUNCTIONAL_CREATOR_HPP

#include <stdexcept>
#include <functional>

#include "creator.hpp"

namespace engine_utl {
	template <typename Signature>
	class FunctionalCreator;

	template <typename Tproduct, typename... Args>
	class FunctionalCreator<Tproduct(Args...)>: public engine::Creator<Tproduct(Args...)> {
	public:
		using CreateFunction = std::function<Tproduct(Args...)>;
		
		FunctionalCreator(const CreateFunction& create_function);
		FunctionalCreator(const FunctionalCreator& other) = default;
		FunctionalCreator& operator=(const FunctionalCreator& other) = default;
				
		Tproduct create(Args... args) const override;
		engine::Creator<Tproduct(Args...)> *clone() const override;
	private:
		CreateFunction m_create_function;
	};

	template <typename Tproduct, typename... Args>
	inline FunctionalCreator<Tproduct(Args...)>::FunctionalCreator(const CreateFunction& create_function): m_create_function(create_function) {
		if (!m_create_function) {
			throw std::invalid_argument("invalid create action received");
		}
	}

	template <typename Tproduct, typename... Args>
	inline Tproduct FunctionalCreator<Tproduct(Args...)>::create(Args... args) const {
		return m_create_function(std::forward<Args>(args)...);
	}

	template <typename Tproduct, typename... Args>
	inline engine::Creator<Tproduct(Args...)> *FunctionalCreator<Tproduct(Args...)>::clone() const {
		return new FunctionalCreator(*this);
	}
}

#endif // FUNCTIONAL_CREATOR_HPP