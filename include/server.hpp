#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <exception>
#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "task.hpp"

namespace server {
	class Server {
	public:
		using ServerTask = Task<Data *(void)>;
		using TaskFactory = Creator<ServerTask *(const Data&)>;
		using FailureReportCreator = Creator<Data *(const std::exception&)>;
		
		Server(const TaskFactory& factory, const FailureReportCreator& failure_report_ctor);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
		virtual ~Server() noexcept = default;

		Data *run(const Data& data) const;
	private:
		std::unique_ptr<TaskFactory> m_factory;
		std::unique_ptr<FailureReportCreator> m_failure_report_ctor;
	};

	inline Server::Server(const TaskFactory& factory, const FailureReportCreator& failure_report_ctor): m_factory(factory.clone()), m_failure_report_ctor(failure_report_ctor.clone()) {

	}

	inline Data *Server::run(const Data& data) const {
		try {
			std::unique_ptr<Task<Data *(void)>> task(m_factory->create(data));
			return task->execute();
		} catch (const std::exception& e) {
			return m_failure_report_ctor->create(e);
		}
	}
}

#endif // SERVER_HPP