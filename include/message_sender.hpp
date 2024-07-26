#ifndef	MESSAGE_SENDER_HPP
#define	MESSAGE_SENDER_HPP

namespace mcu_server {
	
	template <typename Tmsg>
	class MessageSender {
	public:
		virtual ~MessageSender() noexcept = default;
		virtual void send(const Tmsg&) const = 0;
	};
}

#endif // MESSAGE_SENDER_HPP