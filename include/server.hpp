#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <exception>
#include <memory>
#include <stdexcept>

#include "broker.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "listener.hpp"
#include "task.hpp"

namespace server {
	template <typename Tsubscriber_id>
	class Server {
	public:
		using DataBroker = Broker<Tsubscriber_id, Data>;
		using ServerTask = Task<Data *(void)>;
		using TaskFactory = Creator<ServerTask *(const Data&)>;
		using FailureReportCreator = Creator<Data *(const std::exception&)>;
		
		Server(DataBroker *broker, const Tsubscriber_id& id, const TaskFactory& factory, const FailureReportCreator& failure_report_ctor);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;
		virtual ~Server() noexcept;
	private:
		DataBroker *m_broker;
		Tsubscriber_id m_id;
		std::unique_ptr<TaskFactory> m_factory;
		std::unique_ptr<FailureReportCreator> m_failure_report_ctor;

		class DataListener: public Listener<const Data&> {
		public:
			DataListener(Server *server);
			DataListener(const DataListener& other) = default;
			DataListener& operator=(const DataListener& other) = delete;
			void on_event(const Data& data) override;
			Listener<const Data&> *clone() const override;
		private:
			Server *m_server;
		};
	};

	template <typename Tsubscriber_id>
	inline Server<Tsubscriber_id>::Server(DataBroker *broker, const Tsubscriber_id& id, const TaskFactory& factory, const FailureReportCreator& failure_report_ctor): m_broker(broker), m_id(id), m_factory(factory.clone()), m_failure_report_ctor(failure_report_ctor.clone()) {
		if (!m_broker) {
			throw std::invalid_argument("invalid broker pointer");
		}
		m_broker->subscribe(m_id, DataListener(this));
	}

	template <typename Tsubscriber_id>
	inline Server<Tsubscriber_id>::~Server() noexcept {
		m_broker->unsubscribe(m_id);
	}

	template <typename Tsubscriber_id>
	inline Server<Tsubscriber_id>::DataListener::DataListener(Server *server): m_server(server) {
		if (!m_server) {
			throw std::invalid_argument("invalid server pointer");
		}
	}

	template <typename Tsubscriber_id>
	inline void Server<Tsubscriber_id>::DataListener::on_event(const Data& data) {
		try {
			std::unique_ptr<Task<Data *(void)>> task(m_server->m_factory->create(data));
			std::unique_ptr<Data> report(task->execute());
			m_server->m_broker->post(*report);
		} catch (const std::exception& e) {
			std::unique_ptr<Data> report(m_server->m_failure_report_ctor->create(e));
			m_server->m_broker->post(*report);
		}
	}

	template <typename Tsubscriber_id>
	inline Listener<const Data&> *Server<Tsubscriber_id>::DataListener::clone() const {
		return new DataListener(*this);
	}
}

#endif // SERVER_HPP