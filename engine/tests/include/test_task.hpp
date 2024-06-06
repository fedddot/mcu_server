#ifndef	TEST_TASK_HPP
#define	TEST_TASK_HPP

#include <functional>
#include <stdexcept>

#include "task.hpp"

namespace engine_ut {

	template <class Treport>
	class TestTask: public engine::Task<Treport> {
	public:
		using ExecuteAction = std::function<Treport(void)>;
		TestTask(const ExecuteAction& execute_action);
		virtual Treport execute() override;
	private:
		ExecuteAction m_execute_action;
	};

	template <class Treport>
	inline TestTask<Treport>::TestTask(const ExecuteAction& execute_action): m_execute_action(execute_action) {
		if (!m_execute_action) {
			throw std::invalid_argument("invalid execute action received");
		}
	}

	template <class Treport>
	inline Treport TestTask<Treport>::execute() {
		return m_execute_action();
	}
}

#endif // TEST_TASK_HPP