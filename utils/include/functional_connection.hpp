#ifndef	FUNCTIONAL_CONNECTION_HPP
#define	FUNCTIONAL_CONNECTION_HPP

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>

#include "connection.hpp"
#include "listener.hpp"

namespace server_utl {
	template <typename Tid, typename Tdata>
	class FunctionalConnection: public server::Connection<Tid, Tdata> {
	public:
		using SendAction = std::function<void(const Tdata&)>;
		using IdGenerator = std::function<Tid()>;

		FunctionalConnection(const SendAction& send_action, const IdGenerator& id_generator);
		FunctionalConnection(const FunctionalConnection& other) = delete;
		FunctionalConnection& operator=(const FunctionalConnection& other) = delete;

		void send_data(const Tdata& data) const override;
		Tid subscribe(const server::Listener<const Tdata&>& listener) override;
		void unsubscribe(const Tid& listener_id) override;

		void post_data(const Tdata& data);
	private:
		SendAction m_send_action;
		IdGenerator m_id_generator;
		using ListenerUniquePtr = std::unique_ptr<server::Listener<const Tdata&>>;
		std::map<Tid, ListenerUniquePtr> m_listeners;
	};

	template <typename Tid, typename Tdata>
	inline FunctionalConnection<Tid, Tdata>::FunctionalConnection(const SendAction& send_action, const IdGenerator& id_generator): m_send_action(send_action), m_id_generator(id_generator) {

	}

	template <typename Tid, typename Tdata>
	inline void FunctionalConnection<Tid, Tdata>::send_data(const Tdata& data) const {
		m_send_action(data);
	}

	template <typename Tid, typename Tdata>
	inline Tid FunctionalConnection<Tid, Tdata>::subscribe(const server::Listener<const Tdata&>& listener) {
		auto id = m_id_generator();
		if (m_listeners.end() != m_listeners.find(id)) {
			throw std::invalid_argument("listener with received id is already registered");
		}
		m_listeners.insert({id, ListenerUniquePtr(listener.clone())});
		return id;
	}

	template <typename Tid, typename Tdata>
	inline void FunctionalConnection<Tid, Tdata>::unsubscribe(const Tid& listener_id) {
		auto iter = m_listeners.find(listener_id);
		if (m_listeners.end() == iter) {
			throw std::invalid_argument("listener with received id is not registered");
		}
		m_listeners.erase(iter);
	}

	template <typename Tid, typename Tdata>
	inline void FunctionalConnection<Tid, Tdata>::post_data(const Tdata& data) {
		for (auto item: m_listeners) {
			item.second->on_event(data);
		}
	}
}
#endif // FUNCTIONAL_CONNECTION_HPP