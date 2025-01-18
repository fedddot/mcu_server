#ifndef	MANAGER_CONFIG_HPP
#define	MANAGER_CONFIG_HPP

#include <string>

namespace manager {
	class ManagerConfig {
	public:
		virtual ~ManagerConfig() noexcept = default;
		virtual std::string type() const = 0;
	};
}

#endif // MANAGER_CONFIG_HPP