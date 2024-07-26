#ifndef	CUSTOM_TASK_HPP
#define	CUSTOM_TASK_HPP

#include <functional>
#include <utility>

#include "task.hpp"

namespace mcu_server_utl {
	template <typename Signature>
	class CustomTask;

	template <typename Treport, typename... Args>
	class CustomTask<Treport(Args...)>: public mcu_server::Task<Treport(Args...)> {
	public:
		using ExecuteAction = std::function<Treport(Args...)>;
		CustomTask(const ExecuteAction& execute_action);
		CustomTask(const CustomTask& other) = default;
		CustomTask& operator=(const CustomTask& other) = default;

		virtual Treport execute(Args... args) const override;
	private:
		ExecuteAction m_execute_action;
	};

	template <typename Treport, typename... Args>
	inline CustomTask<Treport(Args...)>::CustomTask(const ExecuteAction& execute_action): m_execute_action(execute_action) {

	}

	template <typename Treport, typename... Args>
	inline Treport CustomTask<Treport(Args...)>::execute(Args... args) const {
		return m_execute_action(std::forward<Args>(args)...);
	}
}

#endif // CUSTOM_TASK_HPP