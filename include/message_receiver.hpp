#ifndef	MESSAGE_RECEIVER_HPP
#define	MESSAGE_RECEIVER_HPP

namespace mcu_server {	

	template <typename Tmsg>
	class MessageReceiver {
	public:
		virtual ~MessageReceiver() noexcept = default;
		virtual bool message_received() const = 0;
		virtual Tmsg receive() = 0;
	};
}

#endif // MESSAGE_RECEIVER_HPP