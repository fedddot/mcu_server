#ifndef	ENGINE_HPP
#define	ENGINE_HPP

#include <exception>
#include <memory>

#include "creator.hpp"
#include "task.hpp"

namespace engine {
	template <class Tdata, class Treport>
	class Engine {
	public:
		using EngineTask = Task<Treport>;
		using EngineTaskFactory = Creator<EngineTask *(const Tdata&)>;
		using EngineFailureReportCreator = Creator<Treport(const std::exception&)>;
		
		Engine(const EngineTaskFactory& factory, const EngineFailureReportCreator& failure_report_creator);
		Engine(const Engine& other) = delete;
		Engine& operator=(const Engine& other) = delete;

		~Engine() noexcept = default;

		Treport run_task(const Tdata& cfg);
	private:
		std::unique_ptr<EngineTaskFactory> m_task_factory;
		std::unique_ptr<EngineFailureReportCreator> m_failure_report_creator;
	};

	template <class Tdata, class Treport>
	Engine<Tdata, Treport>::Engine(const EngineTaskFactory& factory, const EngineFailureReportCreator& failure_report_creator): m_task_factory(factory.clone()), m_failure_report_creator(failure_report_creator.clone()) {

	}

	template <class Tdata, class Treport>
	inline Treport Engine<Tdata, Treport>::run_task(const Tdata& cfg) {
		try {
			std::unique_ptr<EngineTask> task_ptr(m_task_factory->create(cfg));
			return task_ptr->execute();
		} catch (const std::exception& e) {
			return m_failure_report_creator->create(e);
		}
	}
}

#endif // ENGINE_HPP