#ifndef	TASK_HPP
#define	TASK_HPP

namespace engine {
	template <class Treport>
	class Task {
	public:
		virtual ~Task() noexcept = default;
		virtual Treport execute() = 0;
	};
}

#endif // TASK_HPP