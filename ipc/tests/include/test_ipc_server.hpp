#ifndef	TEST_IPC_SERVER_HPP
#define	TEST_IPC_SERVER_HPP

#include <functional>
#include <vector>

#include "ipc_server.hpp"

namespace ipc {
	template <typename Request, typename Response>
	class TestIpcServer: public IpcServer<Request, Response> {
	public:
		using RequestCallback = typename IpcServer<Request, Response>::RequestCallback;
		using ResponseReceiver = std::function<void(const Response&)>;
		
		TestIpcServer(const ResponseReceiver& receiver): m_receiver(receiver) {

		}
		TestIpcServer(const TestIpcServer&) = delete;
		TestIpcServer& operator=(const TestIpcServer&) = delete;

		void enqueue_request(const Request& request) {
			m_requests.push_back(request);
		}

		void serve_once(const RequestCallback& request_callback) const override {
			if (m_requests.empty()) {
				return;
			}
			const auto iter = m_requests.begin();
			const auto response = request_callback(*iter);
			m_requests.erase(iter);
			m_receiver(response);
		}
	private:
		ResponseReceiver m_receiver;
		mutable std::vector<Request> m_requests;
	};
}

#endif // TEST_IPC_SERVER_HPP