#ifndef	TASK_CREATOR_HPP
#define	TASK_CREATOR_HPP

#include "creator.hpp"
#include "data.hpp"
#include "task.hpp"

namespace mcu_factory {
	class TaskCreator: public server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> {
	public:
		virtual bool is_creatable(const server::Data& data) const = 0;
		virtual TaskCreator *clone_task_creator() const = 0;
	};
}
#endif // TASK_CREATOR_HPP