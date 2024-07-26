#ifndef	TASK_HPP
#define	TASK_HPP

namespace mcu_server {
	template <typename Signature>
	class Task;

	template <typename Treport, typename... Args>
	class Task<Treport(Args...)> {
	public:
		virtual ~Task() noexcept = default;
		virtual Treport execute(Args...) const = 0;
	};
}

#endif // TASK_HPP