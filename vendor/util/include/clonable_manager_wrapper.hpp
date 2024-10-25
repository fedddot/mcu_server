#ifndef	CLONABLE_MANAGER_WRAPPER_HPP
#define	CLONABLE_MANAGER_WRAPPER_HPP

#include "data.hpp"
#include "manager.hpp"
#include "object.hpp"
#include "resources_vendor.hpp"
#include "server_types.hpp"

namespace vendor {
	class ClonableManagerWrapper: public ResourcesVendor::ClonableManager {
	public:
		ClonableManagerWrapper(server::Manager *manager);
		ClonableManagerWrapper(const ClonableManagerWrapper& other) = default;
		ClonableManagerWrapper& operator=(const ClonableManagerWrapper& other) = delete;
		void create_resource(const server::ResourceId& id, const server::Data& create_cfg) override;
		server::Object read_resource(const server::Path& route) const override;
		server::Object read_all_resources() const override;
		void update_resource(const server::Path& route, const server::Data& update_cfg) override;
		void delete_resource(const server::Path& route) override;
		bool contains(const server::Path& route) const override;
		server::Manager *clone() const override;
	private:
		std::shared_ptr<server::Manager> m_manager;
	};

	inline ClonableManagerWrapper::ClonableManagerWrapper(server::Manager *manager): m_manager(manager) {

	}

	inline void ClonableManagerWrapper::create_resource(const server::ResourceId& id, const server::Data& create_cfg) {
		m_manager->create_resource(id, create_cfg);
	}

	inline server::Object ClonableManagerWrapper::read_resource(const server::Path& route) const {
		return m_manager->read_resource(route);
	}

	inline server::Object ClonableManagerWrapper::read_all_resources() const {
		return m_manager->read_all_resources();
	}

	inline void ClonableManagerWrapper::update_resource(const server::Path& route, const server::Data& update_cfg) {
		m_manager->update_resource(route, update_cfg);
	}

	inline void ClonableManagerWrapper::delete_resource(const server::Path& route) {
		m_manager->delete_resource(route);
	}

	inline bool ClonableManagerWrapper::contains(const server::Path& route) const {
		return m_manager->contains(route);
	}

	inline server::Manager *ClonableManagerWrapper::clone() const {
		return new ClonableManagerWrapper(*this);
	}
}
#endif // CLONABLE_MANAGER_WRAPPER_HPP