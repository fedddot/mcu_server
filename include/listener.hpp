#ifndef	LISTENER_HPP
#define	LISTENER_HPP

namespace mcu_server {	
	template <typename Tevent>
	class Listener {
	public:
		virtual ~Listener() noexcept = default;
		virtual void on_event(const Tevent& event) = 0;
		virtual Listener *clone() const = 0;
	};
}

#endif // LISTENER_HPP