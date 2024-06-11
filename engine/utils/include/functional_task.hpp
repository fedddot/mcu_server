#ifndef	FUNCTIONAL_TASK_HPP
#define	FUNCTIONAL_TASK_HPP

#include <functional>

#include "task.hpp"

namespace engine_utl {

	template <class Treport>
	class FunctionalTask: public engine::Task<Treport> {
	public:
		using ExecuteAction = std::function<Treport(void)>;
		FunctionalTask(const ExecuteAction& execute_action);
		virtual Treport execute() override;
	private:
		ExecuteAction m_execute_action;
	};

	template <class Treport>
	inline FunctionalTask<Treport>::FunctionalTask(const ExecuteAction& execute_action): m_execute_action(execute_action) {

	}

	template <class Treport>
	inline Treport FunctionalTask<Treport>::execute() {
		return m_execute_action();
	}
}

#endif // FUNCTIONAL_TASK_HPP