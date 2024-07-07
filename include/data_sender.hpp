#ifndef	DATA_SENDER_HPP
#define	DATA_SENDER_HPP

namespace mcu_server {
	
	template <typename Tdata>
	class DataSender {
	public:
		virtual ~DataSender() noexcept = default;
		virtual void send(const Tdata&) const = 0;
	};
}

#endif // DATA_SENDER_HPP