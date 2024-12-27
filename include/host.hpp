#ifndef	HOST_HPP
#define	HOST_HPP

#include <functional>
#include <memory>

#include "manager.hpp"

namespace app {
	template <typename Trequest, typename Tresponse, typename Tmanager_cfg, typename Tapp_cfg>
	class Host {
	public:
		using Manager = manager::Manager<Trequest, Tresponse>;
		using ManagerFactory = std::function<Manager *(const Tmanager_cfg&)>;

		Host(const Tapp_cfg& config, const ManagerFactory& manager_factory) {

		}
		virtual ~Host() noexcept = default;
		void run();
		bool is_running() const;
		void stop();
	private:
		std::unique_ptr<Manager> m_manager;
	};
}

#endif // HOST_HPP