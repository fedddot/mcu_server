#ifndef	IPC_CONNECTION_HPP
#define	IPC_CONNECTION_HPP

#include <functional>

namespace ipc {
	template <typename Tsubscriber_id, typename Tincoming, typename Toutgoing>
	class IpcConnection {
	public:
		using Callback = std::function<void(const Tincoming& incoming_data)>;
		virtual ~IpcConnection() noexcept = default;
		virtual void subscribe(const Tsubscriber_id& id, const Callback& cb) = 0;
		virtual void unsubscribe(const Tsubscriber_id& id) = 0;
		virtual bool is_subscribed(const Tsubscriber_id& id) const = 0;
		virtual void send(const Toutgoing& outgoing_data) const = 0;
	};
}

#endif // IPC_CONNECTION_HPP