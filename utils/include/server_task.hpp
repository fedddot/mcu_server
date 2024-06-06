#ifndef	SERVER_TASK_HPP
#define	SERVER_TASK_HPP

#include <functional>

#include "task.hpp"

namespace server_utl {

	template <class Treport>
	class ServerTask: public engine::Task<Treport> {
	public:
		using ExecuteAction = std::function<Treport(void)>;
		ServerTask(const ExecuteAction& execute_action);
		virtual Treport execute() override;
	private:
		ExecuteAction m_execute_action;
	};

	template <class Treport>
	inline ServerTask<Treport>::ServerTask(const ExecuteAction& execute_action): m_execute_action(execute_action) {

	}

	template <class Treport>
	inline Treport ServerTask<Treport>::execute() {
		return m_execute_action();
	}
}

#endif // SERVER_TASK_HPP