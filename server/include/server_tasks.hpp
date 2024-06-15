#ifndef	SERVER_TASKS_HPP
#define	SERVER_TASKS_HPP

namespace server {
	enum class TaskId: int {
		CREATE_GPIO,
		DELETE_GPIO,
		SET_GPIO_STATE,
		READ_GPIO_STATE
	};
}

#endif // SERVER_TASKS_HPP