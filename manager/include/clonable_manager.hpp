#ifndef	CLONABLE_MANAGER_HPP
#define	CLONABLE_MANAGER_HPP

#include "manager.hpp"

namespace manager {
	template <typename Request, typename Response>
	class ClonableManager: public Manager<Request, Response> {
	public:
		virtual Manager<Request, Response> *clone() const = 0;
	};
}

#endif // CLONABLE_MANAGER_HPP