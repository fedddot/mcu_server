#ifndef	TEST_IPC_SERVER_HPP
#define	TEST_IPC_SERVER_HPP

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

#include "ipc_config.hpp"
#include "ipc_option.hpp"
#include "ipc_server.hpp"

namespace ipc_tests {
	template <typename Response>
	class TestIpcConfig: public ipc::IpcConfig {
	public:
		using OnWriteCallback = std::function<void(const Response& data)>;
		TestIpcConfig(const OnWriteCallback& on_write, const std::size_t& timeout_ms): on_write(on_write), timeout_ms(timeout_ms) {

		}
		TestIpcConfig(const TestIpcConfig&) = default;
		TestIpcConfig& operator=(const TestIpcConfig&) = default;
		
		std::string type() const override {
			return std::string("ipc.server.test");
		}

		OnWriteCallback on_write;
		std::size_t timeout_ms;
	};

	template <typename Request, typename Response>
	class TestIpcServer: public ipc::IpcServer<Request, Response> {
	public:
		TestIpcServer(const TestIpcConfig<Response>& config): m_config(config) {

		}
		TestIpcServer(const TestIpcServer&) = delete;
		TestIpcServer& operator=(const TestIpcServer&) = delete;

		ipc::Option<Request> read() override {
			std::unique_lock lock(m_mux);
			if (!m_test_message) {
				m_cond.wait_for(lock, std::chrono::microseconds(m_config.timeout_ms));
			}
			if (!m_test_message) {
				return ipc::Option<Request>(nullptr);
			}
			ipc::Option<Request> result(new Request(*m_test_message));
			m_test_message = nullptr;
			return result;
		}
		void write(const Response& data) const override {
			m_config.on_write(data);
		}
		void publish_test_request(const Request& request) {
			std::unique_lock lock(m_mux);
			if (m_test_message) {
				throw std::runtime_error("there is already message waiting to be read");
			}
			m_test_message = std::make_unique<Request>(request);
			m_cond.notify_one();
		}
	private:
		TestIpcConfig<Response> m_config;
		std::unique_ptr<Request> m_test_message;
		mutable std::mutex m_mux;
		mutable std::condition_variable m_cond;
	};

}

#endif // TEST_IPC_SERVER_HPP