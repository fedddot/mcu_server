#ifndef	BROKER_HPP
#define	BROKER_HPP

#include "listener.hpp"

namespace server {
	template <typename Tsubscriber_id, typename Tdata>
	class Broker {
	public:
		virtual ~Broker() noexcept = default;
		virtual void subscribe(const Tsubscriber_id& id, const Listener<const Tdata&>& listener) = 0;
		virtual void unsubscribe(const Tsubscriber_id& id) = 0;
		virtual bool is_subscribed(const Tsubscriber_id& id) const = 0;
		virtual void post(const Tdata& data) const = 0;
	};
}

#endif // BROKER_HPP