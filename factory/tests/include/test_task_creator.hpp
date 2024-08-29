#ifndef	TEST_TASK_CREATOR_HPP
#define	TEST_TASK_CREATOR_HPP

#include <functional>

#include "creator.hpp"
#include "data.hpp"
#include "task.hpp"
#include "task_creator.hpp"

namespace mcu_factory_uts {
	class TestTaskCreator: public mcu_factory::TaskCreator {
	public:
		using CreateFunction = std::function<server::Task<server::Data *(void)> *(const server::Data&)>;
		using IsCreatableFunction = std::function<bool(const server::Data&)>;
		TestTaskCreator(const CreateFunction& create_function, const IsCreatableFunction& is_creatable_function): m_create_function(create_function), m_is_creatable_function(is_creatable_function) {

		}
		TestTaskCreator(const TestTaskCreator&) = default;
		TestTaskCreator& operator=(const TestTaskCreator&) = delete;
		bool is_creatable(const server::Data& data) const override {
			return m_is_creatable_function(data);
		}
		mcu_factory::TaskCreator *clone_task_creator() const override {
			return new TestTaskCreator(*this);
		}
		server::Task<server::Data *(void)> *create(const server::Data& data) const override {
			return m_create_function(data);
		}
		server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *clone() const override {
			return clone_task_creator();
		}
	private:
		CreateFunction m_create_function;
		IsCreatableFunction m_is_creatable_function;
	};
}
#endif // TEST_TASK_CREATOR_HPP