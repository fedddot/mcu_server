#ifndef	MCU_SERVER_HPP
#define	MCU_SERVER_HPP

#include <exception>
#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "task.hpp"

namespace mcu_server {
	class McuServer {
	public:
		using TaskFactory = Creator<Task<Data *(void)> *(const Data&)>;
		using FailureReportCreator = Creator<Data *(const std::exception&)>;
		
		McuServer(const TaskFactory& factory, const FailureReportCreator& failure_report_ctor);
		McuServer(const McuServer& other) = delete;
		McuServer& operator=(const McuServer& other) = delete;
		virtual ~McuServer() noexcept = default;

		Data *run(const Data& data) const;
	private:
		std::unique_ptr<TaskFactory> m_factory;
		std::unique_ptr<FailureReportCreator> m_failure_report_ctor;
	};

	inline McuServer::McuServer(const TaskFactory& factory, const FailureReportCreator& failure_report_ctor): m_factory(factory.clone()), m_failure_report_ctor(failure_report_ctor.clone()) {

	}

	inline Data *McuServer::run(const Data& data) const {
		try {
			std::unique_ptr<Task<Data *(void)>> task(m_factory->create(data));
			return task->execute();
		} catch (const std::exception& e) {
			return m_failure_report_ctor->create(e);
		}
	}
}

#endif // MCU_SERVER_HPP