#ifndef	DATA_RECEIVER_HPP
#define	DATA_RECEIVER_HPP

#include "listener.hpp"

namespace mcu_server {	

	template <typename Tdata, typename Tfood>
	class DataReceiver {
	public:
		virtual ~DataReceiver() noexcept = default;
		virtual void set_listener(const Listener<Tdata>&) = 0;
		virtual void feed(const Tfood& food) = 0;
	};
}

#endif // DATA_RECEIVER_HPP