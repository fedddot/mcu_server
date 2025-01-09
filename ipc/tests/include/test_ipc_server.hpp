#ifndef	TEST_IPC_SERVER_HPP
#define	TEST_IPC_SERVER_HPP

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>

#include "ipc_server.hpp"

namespace ipc_tests {
	template <typename Tincoming, typename Toutgoing>
	class TestIpcServer: public ipc::IpcServer<Tincoming, Toutgoing> {
	public:
		using OnWriteCallback = std::function<void(const Toutgoing& data)>;
		TestIpcServer(const OnWriteCallback& on_write, const std::size_t& timeout_ms): m_on_write(on_write), m_timeout_ms(timeout_ms) {

		}
		TestIpcServer(const TestIpcServer&) = delete;
		TestIpcServer& operator=(const TestIpcServer&) = delete;
		bool readable() const override {
			std::unique_lock lock(m_mux);
			if (m_test_message) {
				return true;
			}
			m_cond.wait_for(lock, std::chrono::microseconds(m_timeout_ms));
			return m_test_message != nullptr;
		}
		Tincoming read() override {
			std::unique_lock lock(m_mux);
			if (!m_test_message) {
				throw std::runtime_error("there is no message to read");
			}
			Tincoming result(*m_test_message);
			m_test_message = nullptr;
			return result;
		}
		void write(const Toutgoing& data) const override {
			m_on_write(data);
		}
		void publish_test_request(const Tincoming& request) {
			std::unique_lock lock(m_mux);
			if (m_test_message) {
				throw std::runtime_error("there is already message waiting to be read");
			}
			m_test_message = std::make_unique<Tincoming>(request);
			m_cond.notify_one();
		}
	private:
		OnWriteCallback m_on_write;
		std::size_t m_timeout_ms;
		std::unique_ptr<Tincoming> m_test_message;
		mutable std::mutex m_mux;
		mutable std::condition_variable m_cond;
	};

}

#endif // TEST_IPC_SERVER_HPP