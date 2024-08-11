#ifndef	TASK_EXECUTOR_HPP
#define	TASK_EXECUTOR_HPP

namespace cnc {
	template <typename Signature>
	class TaskExecutor;

	template <typename Tdata, typename... Args>
	class TaskExecutor<Tdata(Args...)> {
	public:
		virtual ~TaskExecutor() noexcept = default;
		virtual Tdata execute(Args...) = 0;
		virtual TaskExecutor *clone() const = 0;
	};
}

#endif // TASK_EXECUTOR_HPP