#ifndef	TEST_CREATOR_HPP
#define	TEST_CREATOR_HPP

#include <functional>
#include <stdexcept>

#include "creator.hpp"

namespace engine_ut {
	template <typename Tproduct, typename... Args>
	class TestCreator;

	template <typename Tproduct, typename... Args>
	class TestCreator<Tproduct(Args...)>: public engine::Creator<Tproduct(Args...)> {
	public:
		using CreateFunction = std::function<Tproduct(Args...)>;
		
		TestCreator(const CreateFunction& create_function);
		TestCreator(const TestCreator& other) = default;
		TestCreator& operator=(const TestCreator& other) = default;
				
		Tproduct create(Args... args) const override;
		engine::Creator<Tproduct(Args...)> *clone() const override;
	private:
		CreateFunction m_create_function;
	};

	template <typename Tproduct, typename... Args>
	inline TestCreator<Tproduct(Args...)>::TestCreator(const CreateFunction& create_function): m_create_function(create_function) {
		if (!m_create_function) {
			throw std::invalid_argument("invalid create action received");
		}
	}

	template <typename Tproduct, typename... Args>
	inline Tproduct TestCreator<Tproduct(Args...)>::create(Args... args) const {
		return m_create_function(std::forward<Args>(args)...);
	}

	template <typename Tproduct, typename... Args>
	inline engine::Creator<Tproduct(Args...)> *TestCreator<Tproduct(Args...)>::clone() const {
		return new TestCreator(*this);
	}
}

#endif // TEST_CREATOR_HPP