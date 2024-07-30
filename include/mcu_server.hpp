#ifndef	MCU_SERVER_HPP
#define	MCU_SERVER_HPP

#include <exception>
#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "data.hpp"
#include "message_receiver.hpp"
#include "message_sender.hpp"
#include "parser.hpp"
#include "serializer.hpp"
#include "task.hpp"

namespace mcu_server {
	template <typename Traw_data>
	class McuServer {
	public:
		using McuMessageSender = mcu_platform::MessageSender<Traw_data>;
		using McuMessageReceiver = mcu_platform::MessageReceiver<Traw_data>;
		using McuParser = Parser<Data *(const Traw_data&)>;
		using McuSerializer = Serializer<Traw_data(const Data&)>;
		using TaskFactory = Creator<Task<Data *(void)> *(const Data&)>;
		using FailureReportCreator = Creator<Data *(const std::exception&)>;
		
		McuServer(McuMessageSender *sender, McuMessageReceiver *receiver, const McuParser& parser, const McuSerializer& serializer, const TaskFactory& factory, const FailureReportCreator& failure_report_ctor);
		McuServer(const McuServer& other) = delete;
		McuServer& operator=(const McuServer& other) = delete;
		virtual ~McuServer() noexcept;

		void run();
		bool is_running() const;
		void stop();
	private:
		McuMessageSender *m_sender;
		McuMessageReceiver *m_receiver;
		std::unique_ptr<McuParser> m_parser;
		std::unique_ptr<McuSerializer> m_serializer;
		std::unique_ptr<TaskFactory> m_factory;
		std::unique_ptr<FailureReportCreator> m_failure_report_ctor;

		bool m_is_running;

		void run_server_iteration();
	};

	template <typename Traw_data>
	inline McuServer<Traw_data>::McuServer(McuMessageSender *sender, McuMessageReceiver *receiver, const McuParser& parser, const McuSerializer& serializer, const TaskFactory& factory, const FailureReportCreator& failure_report_ctor): m_sender(sender), m_receiver(receiver), m_parser(parser.clone()), m_serializer(serializer.clone()), m_factory(factory.clone()), m_failure_report_ctor(failure_report_ctor.clone()), m_is_running(false) {
		if (!sender || !receiver) {
			throw std::invalid_argument("invalid ptr received");
		}
	}

	template <typename Traw_data>
	inline McuServer<Traw_data>::~McuServer() noexcept {
		if (is_running()) {
			stop();
		}
	}

	template <typename Traw_data>
	inline void McuServer<Traw_data>::run() {
		if (m_is_running) {
			throw std::runtime_error("the server is already running");
		}
		m_is_running = true;
		while (m_is_running) {
			run_server_iteration();
		}
	}

	template <typename Traw_data>
	inline bool McuServer<Traw_data>::is_running() const {
		return m_is_running;
	}

	template <typename Traw_data>
	inline void McuServer<Traw_data>::stop() {
		m_is_running = false;
	}

	template <typename Traw_data>
	inline void McuServer<Traw_data>::run_server_iteration() {
		std::unique_ptr<Data> report(nullptr);
		try {
			if (!(m_receiver->message_received())) {
				return;
			}
			auto msg = m_receiver->receive();
			std::unique_ptr<Data> parsed_msg(m_parser->parse(msg));
			std::unique_ptr<Task<Data *(void)>> task(m_factory->create(*parsed_msg));
			report = std::unique_ptr<Data>(task->execute());
		} catch (const std::exception& e) {
			report = std::unique_ptr<Data>(m_failure_report_ctor->create(e));
		}
		auto serialized_report = m_serializer->serialize(*report);
		m_sender->send(serialized_report);
	}
}

#endif // MCU_SERVER_HPP