#ifndef	MANAGER_CLONABLE_HPP
#define	MANAGER_CLONABLE_HPP

namespace manager {
	template <typename T>
	class ManagerClonable {
	public:
		virtual ~ManagerClonable() noexcept = default;
		virtual T *clone() const = 0;
	};
}

#endif // MANAGER_CLONABLE_HPP