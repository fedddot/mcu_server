#ifndef	DATA_SENDER_HPP
#define	DATA_SENDER_HPP

namespace mcu_task_engine {
	
	template <typename... Args>
	class DataSender {
	public:
		virtual ~DataSender() noexcept = default;
		virtual void send(Args...) const = 0;
		virtual DataSender *clone() const = 0;
	};
}

#endif // DATA_SENDER_HPP