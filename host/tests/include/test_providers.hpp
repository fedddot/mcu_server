#ifndef	TEST_PROVIDERS_HPP
#define	TEST_PROVIDERS_HPP

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>

#include "provider.hpp"
#include "providers.hpp"

namespace host_tests {
	template <typename Tprovider_id>
	class TestProviders: public manager::Providers<Tprovider_id> {
	public:
		TestProviders() = default;
		TestProviders(const TestProviders&) = delete;
		TestProviders& operator=(const TestProviders&) = delete;
		
		manager::Provider& access_provider(const Tprovider_id& id) const override {
			const auto iter = m_providers.find(id);
			if (m_providers.end() == iter) {
				throw std::invalid_argument("provider not found");
			}
			return std::ref(*(iter->second));
		}
		void add_provider(const Tprovider_id& id, manager::Provider *raw_provider_ptr) {
			m_providers[id] = std::unique_ptr<manager::Provider>(raw_provider_ptr);
		}
	private:
		mutable std::map<Tprovider_id, std::unique_ptr<manager::Provider>> m_providers;
	};
}

#endif // TEST_PROVIDERS_HPP