#ifndef	HOST_HPP
#define	HOST_HPP

#include <exception>
#include <functional>
#include <stdexcept>

#include "request_reader.hpp"
#include "response_writer.hpp"
#include "manager.hpp"

namespace host {
	template <typename Request, typename Response>
	class Host {
	public:
		using FailureReporter = std::function<Response(const std::exception&)>;
		Host(
			ipc::RequestReader<Request> *request_reader,
			ipc::ResponseWriter<Response> *response_writer,
			manager::Manager<Request, Response> *manager,
			const FailureReporter& failure_reporter
		);
		Host(const Host&) = delete;
		Host& operator=(const Host&) = delete;
		virtual ~Host() noexcept = default;
		
		void run_once();
	private:
		ipc::RequestReader<Request> *m_request_reader;
		ipc::ResponseWriter<Response> *m_response_writer;
		manager::Manager<Request, Response> *m_manager;
		const FailureReporter m_failure_reporter;
		const FailureReporter m_m_failure_reporter;
	};

	template <typename Request, typename Response>
	inline Host<Request, Response>::Host(
		ipc::RequestReader<Request> *request_reader,
		ipc::ResponseWriter<Response> *response_writer,
		manager::Manager<Request, Response> *manager,
		const FailureReporter& failure_reporter
	): m_request_reader(request_reader), m_response_writer(response_writer), m_manager(manager), m_failure_reporter(failure_reporter) {
		if (!m_request_reader || !m_response_writer || !m_manager || !m_failure_reporter) {
			throw std::invalid_argument("invalid host args received");
		}
	}

	template <typename Request, typename Response>
	inline void Host<Request, Response>::run_once() {
		try {
			const auto request = m_request_reader->read();
			if (!request) {
				return;
			}
			const auto response = m_manager->run(*request);
			m_response_writer->write(response);
		} catch (const std::exception& e) {
			const auto response = m_failure_reporter(e);
			m_response_writer->write(response);
		}
	}
}

#endif // HOST_HPP