#ifndef	SERVER_CREATOR_HPP
#define	SERVER_CREATOR_HPP

#include <stdexcept>
#include <functional>

#include "creator.hpp"

namespace server_utl {
	template <typename Tproduct, typename... Args>
	class ServerCreator;

	template <typename Tproduct, typename... Args>
	class ServerCreator<Tproduct(Args...)>: public engine::Creator<Tproduct(Args...)> {
	public:
		using CreateFunction = std::function<Tproduct(Args...)>;
		
		ServerCreator(const CreateFunction& create_function);
		ServerCreator(const ServerCreator& other) = default;
		ServerCreator& operator=(const ServerCreator& other) = default;
				
		Tproduct create(Args... args) const override;
		engine::Creator<Tproduct(Args...)> *clone() const override;
	private:
		CreateFunction m_create_function;
	};

	template <typename Tproduct, typename... Args>
	inline ServerCreator<Tproduct(Args...)>::ServerCreator(const CreateFunction& create_function): m_create_function(create_function) {
		if (!m_create_function) {
			throw std::invalid_argument("invalid create action received");
		}
	}

	template <typename Tproduct, typename... Args>
	inline Tproduct ServerCreator<Tproduct(Args...)>::create(Args... args) const {
		return m_create_function(std::forward<Args>(args)...);
	}

	template <typename Tproduct, typename... Args>
	inline engine::Creator<Tproduct(Args...)> *ServerCreator<Tproduct(Args...)>::clone() const {
		return new ServerCreator(*this);
	}
}

#endif // SERVER_CREATOR_HPP