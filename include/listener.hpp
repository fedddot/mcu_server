#ifndef	LISTENER_HPP
#define	LISTENER_HPP

namespace mcu_server {	
	template <typename... Args>
	class Listener {
	public:
		virtual ~Listener() noexcept = default;
		virtual void on_event(Args... args) = 0;
		virtual Listener *clone() const = 0;
	};
}

#endif // LISTENER_HPP