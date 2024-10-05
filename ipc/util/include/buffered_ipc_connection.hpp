#ifndef	BUFFERED_IPC_CONNECTION_HPP
#define	BUFFERED_IPC_CONNECTION_HPP

#include <functional>
#include <map>
#include <stdexcept>

#include "ipc_connection.hpp"
#include "request.hpp"
#include "response.hpp"

namespace ipc_utl {
	template <typename Tsubscriber_id, typename Traw_data>
	class BufferedIpcConnection: public ipc::IpcConnection<Tsubscriber_id, server::Request, server::Response> {
	public:
		using Callback = typename ipc::IpcConnection<Tsubscriber_id, server::Request, server::Response>::Callback;
		
		using RequestMatcher = std::function<bool(const Traw_data&)>;
		using RequestExtractor = std::function<server::Request(Traw_data *)>;
		using ResponseSender = std::function<void(const server::Response&)>;

		BufferedIpcConnection(
			const RequestMatcher& request_matcher,
			const RequestExtractor& request_extractor,
			const ResponseSender& response_sender
		);
		BufferedIpcConnection(const BufferedIpcConnection& other) = delete;
		BufferedIpcConnection& operator=(const BufferedIpcConnection& other) = delete;

		void subscribe(const Tsubscriber_id& id, const Callback& cb) override;
		void unsubscribe(const Tsubscriber_id& id) override;
		bool is_subscribed(const Tsubscriber_id& id) const override;
		void send(const server::Response& outgoing_data) const override;

		void feed(const Traw_data& data);
	private:
		RequestMatcher m_request_matcher;
		RequestExtractor m_request_extractor;
		ResponseSender m_response_sender;

		std::map<Tsubscriber_id, Callback> m_callbacks;

		Traw_data m_data;

		void publish_request(const server::Request& request) const;
	};

	template <typename Tsubscriber_id, typename Traw_data>
	inline BufferedIpcConnection<Tsubscriber_id, Traw_data>::BufferedIpcConnection(
		const RequestMatcher& request_matcher,
		const RequestExtractor& request_extractor,
		const ResponseSender& response_sender
	): m_request_matcher(request_matcher), m_request_extractor(request_extractor), m_response_sender(response_sender) {
		if (!m_request_matcher || !m_request_extractor || !m_response_sender) {
			throw std::invalid_argument("invalid action received");
		}
	}

	template <typename Tsubscriber_id, typename Traw_data>
	inline void BufferedIpcConnection<Tsubscriber_id, Traw_data>::subscribe(const Tsubscriber_id& id, const Callback& cb) {
		if (is_subscribed(id)) {
			throw std::invalid_argument("received id is already subscribed");
		}
		m_callbacks.insert({id, cb});
	}
	
	template <typename Tsubscriber_id, typename Traw_data>
	inline bool BufferedIpcConnection<Tsubscriber_id, Traw_data>::is_subscribed(const Tsubscriber_id& id) const {
		return m_callbacks.end() != m_callbacks.find(id);
	}

	template <typename Tsubscriber_id, typename Traw_data>
	inline void BufferedIpcConnection<Tsubscriber_id, Traw_data>::unsubscribe(const Tsubscriber_id& id) {
		const auto iter = m_callbacks.find(id);
		if (m_callbacks.end() == iter) {
			throw std::invalid_argument("received id is not subscribed");
		}
		m_callbacks.erase(iter);
	}

	template <typename Tsubscriber_id, typename Traw_data>
	inline void BufferedIpcConnection<Tsubscriber_id, Traw_data>::send(const server::Response& outgoing_data) const {
		m_response_sender(outgoing_data);
	}

	template <typename Tsubscriber_id, typename Traw_data>
	inline void BufferedIpcConnection<Tsubscriber_id, Traw_data>::feed(const Traw_data& raw_data) {
		m_data.insert(m_data.end(), raw_data.begin(), raw_data.end());
		while (m_request_matcher(m_data)) {
			publish_request(m_request_extractor(&m_data));
		}
	}

	template <typename Tsubscriber_id, typename Traw_data>
	void BufferedIpcConnection<Tsubscriber_id, Traw_data>::publish_request(const server::Request& request) const {
		for (auto [id, cb]: m_callbacks) {
			(void)id;
			cb(request);
		}
	}
}

#endif // BUFFERED_IPC_CONNECTION_HPP