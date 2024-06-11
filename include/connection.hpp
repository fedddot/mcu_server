#ifndef	CONNECTION_HPP
#define	CONNECTION_HPP

#include "listener.hpp"

namespace mcu_control {
	template <typename Tid, typename Tdata>
	class Connection {
	public:
		virtual ~Connection() noexcept = default;
		virtual void send_data(const Tdata& data) const = 0;
		virtual Tid subscribe(const Listener<const Tdata&>& listener) = 0;
		virtual void unsubscribe(const Tid& listener_id) = 0;
		virtual bool is_subscribed(const Tid& listener_id) const = 0;
	};
}
#endif // CONNECTION_HPP