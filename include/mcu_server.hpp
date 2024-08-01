#ifndef	MCU_SERVER_HPP
#define	MCU_SERVER_HPP

#include <exception>
#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "parser.hpp"
#include "serializer.hpp"
#include "task.hpp"

namespace mcu_server {
	template <typename Tdata>
	class McuServer {
	public:
		using McuParser = Parser<Data *(const Tdata&)>;
		using McuSerializer = Serializer<Tdata(const Data&)>;
		using TaskFactory = Creator<Task<Data *(void)> *(const Data&)>;
		using FailureReportCreator = Creator<Data *(const std::exception&)>;
		
		McuServer(const McuParser& parser, const McuSerializer& serializer, const TaskFactory& factory, const FailureReportCreator& failure_report_ctor);
		McuServer(const McuServer& other) = delete;
		McuServer& operator=(const McuServer& other) = delete;
		virtual ~McuServer() noexcept = default;

		Tdata run(const Tdata& data);
	private:
		std::unique_ptr<McuParser> m_parser;
		std::unique_ptr<McuSerializer> m_serializer;
		std::unique_ptr<TaskFactory> m_factory;
		std::unique_ptr<FailureReportCreator> m_failure_report_ctor;
	};

	template <typename Traw_data>
	inline McuServer<Traw_data>::McuServer(const McuParser& parser, const McuSerializer& serializer, const TaskFactory& factory, const FailureReportCreator& failure_report_ctor): m_parser(parser.clone()), m_serializer(serializer.clone()), m_factory(factory.clone()), m_failure_report_ctor(failure_report_ctor.clone()) {

	}

	template <typename Traw_data>
	inline Traw_data McuServer<Traw_data>::run(const Traw_data& data) {
		try {
			std::unique_ptr<Data> parsed_msg(m_parser->parse(data));
			std::unique_ptr<Task<Data *(void)>> task(m_factory->create(*parsed_msg));
			std::unique_ptr<Data> report(task->execute());
			return m_serializer->serialize(*report);
		} catch (const std::exception& e) {
			std::unique_ptr<Data> report(m_failure_report_ctor->create(e));
			return m_serializer->serialize(*report);
		}
	}
}

#endif // MCU_SERVER_HPP