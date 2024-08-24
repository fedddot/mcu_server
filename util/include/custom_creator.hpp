#ifndef	CUSTOM_CREATOR_HPP
#define	CUSTOM_CREATOR_HPP

#include <stdexcept>
#include <functional>

#include "creator.hpp"

namespace server_utl {
	template <typename Signature>
	class CustomCreator;

	template <typename Tproduct, typename... Args>
	class CustomCreator<Tproduct(Args...)>: public server::Creator<Tproduct(Args...)> {
	public:
		using CreateFunction = std::function<Tproduct(Args...)>;
		
		CustomCreator(const CreateFunction& create_function);
		CustomCreator(const CustomCreator& other) = default;
		CustomCreator& operator=(const CustomCreator& other) = default;
				
		Tproduct create(Args... args) const override;
		server::Creator<Tproduct(Args...)> *clone() const override;
	private:
		CreateFunction m_create_function;
	};

	template <typename Tproduct, typename... Args>
	inline CustomCreator<Tproduct(Args...)>::CustomCreator(const CreateFunction& create_function): m_create_function(create_function) {
		if (!m_create_function) {
			throw std::invalid_argument("invalid create action received");
		}
	}

	template <typename Tproduct, typename... Args>
	inline Tproduct CustomCreator<Tproduct(Args...)>::create(Args... args) const {
		return m_create_function(std::forward<Args>(args)...);
	}

	template <typename Tproduct, typename... Args>
	inline server::Creator<Tproduct(Args...)> *CustomCreator<Tproduct(Args...)>::clone() const {
		return new CustomCreator(*this);
	}
}

#endif // CUSTOM_CREATOR_HPP